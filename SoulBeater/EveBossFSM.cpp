#include "EveBossFSM.h"

#include "SoulBeaterProcessor.h"
#include "ObjectHelper.h"

#include "BeatManager.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "EventManager.h"

#include "IncludeComponent.h"
#include "GameObject.h"
#include "EveBossComponent.h"
#include "DaggerComponent.h"
#include "EveDaggerComponent.h"
#include "BaseMonsterComponent.h"
#include "YsBossComponent.h"

#include "MonsterFSM.h"
#include "ConstantTable.h"

#include "eSoulBeaterID.h"

#include <random>

#include "SoundManager.h"

namespace soulBeater
{
	EveBossFSM::EveBossFSM()
		: BaseEntity(static_cast<unsigned int>(eSoulBeaterID::EveFSM))
		, mBeatManager(nullptr)
		, mEveID(static_cast<unsigned int>(-1))
		, mYsID(static_cast<unsigned int>(-1))
		, mPlayerID(static_cast<unsigned int>(-1))
		, mPrevPlayerY()
		, mCurrentState(eBossState::Idle)
		, mNextState(eBossState::Idle)
		, mBossTurn(eBossTurn::StayTurn)
		, mbIsAttackComplete(false)
		, mDelayBeat(0)
		, mRandAttack(0)
		, mCurrentPhase(ePhase::None)
		, mNextPhase(ePhase::None)
		, mMonsterFSM(nullptr)
	{
	}

	EveBossFSM::~EveBossFSM()
	{
	}

	void EveBossFSM::Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int ysID, unsigned int eveID, unsigned int playerID)
	{
		using namespace d2dFramework;
		srand(static_cast<unsigned int>(time(nullptr)));

		mBeatManager = beatmanager;
		mMonsterFSM = monsterFSM;
		mYsID = ysID;
		mEveID = eveID;
		mPlayerID = playerID;
	}

	void EveBossFSM::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);

		if (bossEve == nullptr || bossYs == nullptr)
		{
			mNextPhase = ePhase::None;
			mCurrentPhase = ePhase::None;
			mStartBeat = 0;
			return;
		}
		if (player == nullptr)
		{
			return;
		}

		update(deltaTime);
		handleState(deltaTime);

		if (mCurrentState != mNextState)
		{
			exit();
			mCurrentState = mNextState;
			enter();
		}

		handlePhaseState();

		if (mCurrentPhase != mNextPhase)
		{
			mCurrentPhase = mNextPhase;
			enterPhase();
		}
	}

	void EveBossFSM::update(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);
		assert(bossEve != nullptr);
		assert(player != nullptr);

		static unsigned int CurrentBeat = 0;
		static unsigned int NextBeat = 0;

		YsBossComponent* BossYsComponent = bossYs->GetComponent<YsBossComponent>();
		EveBossComponent* BossEveComponent = bossEve->GetComponent<EveBossComponent>();
		assert(BossEveComponent != nullptr);

		BossEveComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossEveComponent->GetAccumulateCount(static_cast<float>(mBeatManager->GetIntervalTime()));

		// 액션 턴과 한 박자 쉬는 턴으로 구분
		if (CurrentBeat != NextBeat)
		{
			CurrentBeat = NextBeat;

			if ((int)mBossTurn <= 2)
			{
				mBossTurn = (eBossTurn)((int)mBossTurn + 1);
			}
			else
			{
				mBossTurn = eBossTurn::StayTurn;
			}
			
			static bool Once = 0;

			if (Once)
			{
				Once = false;
				mStartBeat = BossYsComponent->GetBeatCount();
			}

			switch (mCurrentPhase)
			{
			case ePhase::None:
				break;
			case ePhase::One:
			{

			}
			break;
			case ePhase::Two:
			{
				if (mStartBeat + 9 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (mStartBeat + 10 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (mStartBeat + 11 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
					attackPatton();
				}
				else if (mStartBeat + 12 <= BossYsComponent->GetBeatCount())
				{
					Once = true;
				}
			}
			break;
			case ePhase::Three:
			{
				if (mStartBeat + 9 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (mStartBeat + 10 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (mStartBeat + 11 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
					attackPatton();
				}
				if (mStartBeat + 13 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (mStartBeat + 14 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (mStartBeat + 15 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
					attackPatton();
				}
				else if (mStartBeat + 16 <= BossYsComponent->GetBeatCount())
				{
					Once = true;
				}
				else if (mCurrentPhase == ePhase::Four || mCurrentPhase == ePhase::Ending)
				{
					mStartBeat = 0;
					CurrentBeat = 0;
				}
			}
			break;
			case ePhase::Four:
				Once = true;
				mStartBeat = 0;
				CurrentBeat = 0;
				break;
			case ePhase::Ending:
				mStartBeat = 0;
				CurrentBeat = 0;
				Once = true;
				break;
			default:
				break;
			}
		}

		switch (mCurrentState)
		{
		case eBossState::Idle:
			break;
		case eBossState::Trace:
			break;
		case eBossState::AttackStandby:
			break;
		case eBossState::Attack:
			break;
		case eBossState::Death:
			break;
		}
	}

	void EveBossFSM::handleState(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);

		assert(bossEve != nullptr);
		assert(player != nullptr);

		switch (mCurrentState)
		{
		case eBossState::Idle:
		{
		}
		break;
		case eBossState::Trace:
		{
			GridMoveComponent* girdmove = bossEve->GetComponent<GridMoveComponent>();

			if (girdmove->GetIsCanMove())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::AttackStandby:
		{
		}
		break;
		case eBossState::Attack:
		{
			GridMoveComponent* girdmove = bossEve->GetComponent<GridMoveComponent>();

			if (girdmove->GetIsCanMove())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::BeAttacked:
		{
			AnimationRenderer* animation = bossEve->GetComponent<AnimationRenderer>();

			if (animation->GetIsAnimationEnd())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::Death:
		{
			bossEve->GetComponent<CircleCollider>()->SetIsTrigger(false);

			if (bossEve->GetComponent<AnimationRenderer>()->GetIsAnimationEnd())
			{
				mNextState = eBossState::Idle;
				mCurrentState = eBossState::Idle;
				mNextPhase = ePhase::None;
				mCurrentPhase = ePhase::None;

				YsBossComponent* yscomponent = bossYs->GetComponent<YsBossComponent>();
				yscomponent->SetBeatCount(0);

				ObjectManager::GetInstance()->DeleteObject(mEveID);
				ObjectManager::GetInstance()->DeleteObject(mYsID);
			}
		}
		break;
		}
	}

	void EveBossFSM::enter()
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);
		assert(bossEve != nullptr);
		assert(bossYs != nullptr);
		assert(player != nullptr);

		AnimationRenderer* EveAnimation = bossEve->GetComponent<AnimationRenderer>();
		EveBossComponent* YsAndEveComponent = bossEve->GetComponent<EveBossComponent>();

		switch (mCurrentState)
		{
		case eBossState::Idle:			// 가만히
		{
			EveAnimation->SetAnimationIndex(1);
			EveAnimation->SetFrameindex(0);
			EveAnimation->SetIsLoop(true);

			if (player->GetComponent<GridComponent>()->GetPosition().X >= bossEve->GetComponent<GridComponent>()->GetPosition().X)
				EveAnimation->SetIsLeft(true);
			else
				EveAnimation->SetIsLeft(false);
		}
		break;
		case eBossState::Trace:		// y좌표 추격
		{
			EveAnimation->SetAnimationIndex(0);
			EveAnimation->SetFrameindex(0);
			EveAnimation->SetIsLoop(true);

			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = bossEve->GetComponent<GridComponent>();
			GridMoveComponent* movegridcomponent = bossYs->GetComponent<GridMoveComponent>();

			// 플레이어가 보스보다 왼쪽에 있을 경우 다른 보스와 같은 X위치에 없을 때
			if (playergridcomponent->GetPosition().Y > bossgridcomponent->GetPosition().Y
				&& movegridcomponent->GetCurrentPosition().X != bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossEve->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, 1);
			}
			// 같은 X 좌표일 때, 다른 보스의 다음 Y값 계산해서 이동
			else if (playergridcomponent->GetPosition().Y > bossgridcomponent->GetPosition().Y
				&& movegridcomponent->GetNextPosition().Y != bossgridcomponent->GetPosition().Y + 1
				&& movegridcomponent->GetCurrentPosition().Y != bossgridcomponent->GetPosition().Y + 1
				&& movegridcomponent->GetCurrentPosition().X == bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossEve->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, 1);
			}


			// 플레이어가 보스보다 왼쪽에 있을 경우 다른 보스와 같은 X위치에 없을 때
			if (playergridcomponent->GetPosition().Y < bossgridcomponent->GetPosition().Y
				&& movegridcomponent->GetCurrentPosition().X != bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossEve->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, -1);
			}
			// 같은 X 좌표일 때, 다른 보스의 다음 Y값 계산해서 이동
			else if (playergridcomponent->GetPosition().Y < bossgridcomponent->GetPosition().Y
				&& movegridcomponent->GetNextPosition().Y != bossgridcomponent->GetPosition().Y - 1
				&& movegridcomponent->GetCurrentPosition().Y != bossgridcomponent->GetPosition().Y - 1
				&& movegridcomponent->GetCurrentPosition().X == bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossEve->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, -1);
			}
		}
		break;
		case eBossState::AttackStandby:
		{
			GridComponent* playergrid = player->GetComponent<GridComponent>();
			GridComponent* bossgrid = bossEve->GetComponent<GridComponent>();

			GameObject* Warning = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::GetTempId(), static_cast<float>(mBeatManager->GetIntervalTime()));
			AnimationRenderer* sprite = Warning->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
			GridComponent* grid = Warning->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
			Transform* transform = Warning->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());

			Warning->SetObjectType(eObjectType::Item);
			sprite->SetSize({ 3000, 500 });
			sprite->SetTransformLayer(eTransformLayer::Grid);
			sprite->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(WARNING_ANI_KEY));

			grid->SetMaxX(playergrid->GetMaxX());
			grid->SetMaxY(playergrid->GetMaxY());
			grid->SetCellDistance(playergrid->GetCellDistance());
			grid->SetPosition({ playergrid->GetPosition().X, playergrid->GetPosition().Y });

			mPrevPlayerY = playergrid->GetPosition().Y;
		}
		break;
		case eBossState::Attack:
		{
			EveAnimation->SetAnimationIndex(2);
			EveAnimation->SetFrameindex(0);
			EveAnimation->SetIsLoop(true);

			mbIsAttackComplete = true;

		}
		break;
		case eBossState::BeAttacked:
		{
			SoundManager::GetInstance()->Play(EFFECT_EVEHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 4);

			// 피격 당할시 랜덤 위치로 이동
			EveAnimation->SetAnimationIndex(3);
			EveAnimation->SetFrameindex(0);
			EveAnimation->SetIsLoop(false);
		}
		break;
		case eBossState::Death:
			EveAnimation->SetAnimationIndex(4);
			EveAnimation->SetFrameindex(0);
			EveAnimation->SetIsLoop(false);
			break;
		}
	}

	void EveBossFSM::exit()
	{
		switch (mCurrentState)
		{
		case eBossState::Idle:
			break;
		case eBossState::Trace:
			break;
		case eBossState::AttackStandby:
			break;
		case eBossState::Attack:
			break;
		case eBossState::Death:
			break;
		}
	}

	void EveBossFSM::handlePhaseState()
	{
		switch (mCurrentPhase)
		{
		case ePhase::None:
			mNextPhase = ePhase::One;
			break;
		case ePhase::One:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				mNextPhase = ePhase::Two;
			}
			break;
		case ePhase::Two:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				mNextPhase = ePhase::Three;
			}
			break;
		case ePhase::Three:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				mNextPhase = ePhase::Four;
			}
			break;
		case ePhase::Four:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				mNextPhase = ePhase::Ending;
			}
			break;
		case ePhase::Ending:
			// ending이 종료되면
			break;
		default:
			assert(false);
			break;
		}
	}

	void EveBossFSM::enterPhase()
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		GridComponent* grid = nullptr;



		switch (mCurrentPhase)
		{
		case ePhase::None:
			break;
		case ePhase::One:
		{
			ObjectHelper::CreateMonster({ 4, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 2 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 6, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 8, 4 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 8, 6 }, eMonsterType::Hold);

			break;
		}
		case ePhase::Two:
		{
			ObjectHelper::CreateMonster({ 2, 2 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 4, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 6 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 7, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 9, 2 }, eMonsterType::Attack);

			mNextState = eBossState::BeAttacked;

			break;
		}
		break;
		case ePhase::Three:
		{
			ObjectHelper::CreateMonster({ 2, 3 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 4, 2 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 8, 5 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 8, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 9, 6 }, eMonsterType::Escape);

			mNextState = eBossState::BeAttacked;

			break;
		}
		case ePhase::Four:
		{
			mNextState = eBossState::Death;

			break;
		}
		case ePhase::Ending:
		{
			mNextState = eBossState::Death;

			break;
		}
		default:
			assert(false);
			break;
		}
	}

	void EveBossFSM::attackPatton()
	{
		static unsigned int daggerid = 110000;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);

		GridComponent* playergridcomponent = player->GetComponent<GridComponent>();

		GameObject* dagger = ObjectManager::GetInstance()->CreateObject(daggerid++);
		Transform* transform = dagger->CreateComponent<Transform>(daggerid++);
		SpriteRenderer* sprite = dagger->CreateComponent<SpriteRenderer>(daggerid++);
		GridComponent* grid = dagger->CreateComponent<GridComponent>(daggerid++);
		GridMoveComponent* gridmove = dagger->CreateComponent<GridMoveComponent>(daggerid++);
		CircleCollider* collider = dagger->CreateComponent<CircleCollider>(daggerid++);
		EveDaggerComponent* daggerComponent = dagger->CreateComponent<EveDaggerComponent>(daggerid++);

		dagger->SetObjectType(eObjectType::Projectile);

		grid->SetMaxX(MAX_X);
		grid->SetMaxY(MAX_Y);
		grid->SetCellDistance(GRID_DISTANCE);
		sprite->SetSpriteType(eSpriteType::Sprite);
		sprite->SetSize({ (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
		sprite->SetUVRectangle({ 0,0,600,600 });
		sprite->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Blade"));
		sprite->SetIsLeft(true);
		sprite->SetTransformLayer(eTransformLayer::Grid);
		daggerComponent->SetBeatManager(mBeatManager);

		gridmove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		collider->SetRadius(50.f);
		collider->SetIsTrigger(true);
		grid->SetPosition({ MAX_X - 1, mPrevPlayerY });
	}

	void EveBossFSM::Release()
	{

	}
}

