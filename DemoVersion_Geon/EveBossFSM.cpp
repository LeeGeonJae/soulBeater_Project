#include "EveBossFSM.h"

#include "SoulBeaterProcessor.h"

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

#include <random>

namespace TestGeon
{
	EveBossFSM::EveBossFSM()
		: d2dFramework::BaseEntity(90001)
		, mBeatManager(nullptr)
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
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);

		if (bossEve == nullptr)
		{
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

		static int CurrentBeat = 0;
		static int NextBeat = 0;
		
		YsBossComponent* BossYsComponent = bossYs->GetComponent<YsBossComponent>();
		EveBossComponent* BossEveComponent = bossEve->GetComponent<EveBossComponent>();
		assert(BossEveComponent != nullptr);

		BossEveComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossEveComponent->GetAccumulateCount(mBeatManager->GetIntervalTime());

		// 액션 턴과 한 박자 쉬는 턴으로 구분
		if (CurrentBeat < NextBeat)
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

			static int StartBeat = 0;
			static bool Once = 0;

			if (Once)
			{
				Once = false;
				StartBeat = BossYsComponent->GetBeatCount();
			}

			switch (mCurrentPhase)
			{
			case TestGeon::ePhase::None:
				break;
			case TestGeon::ePhase::One:
			{

			}
				break;
			case TestGeon::ePhase::Two:
			{
				if (StartBeat + 9 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 10 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 11 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
					attackPatton();
				}
				else if (StartBeat + 12 <= BossYsComponent->GetBeatCount())
				{
					Once = true;
				}
			}
			break;
			case TestGeon::ePhase::Three:
			{
				if (StartBeat + 9 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 10 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 11 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
					attackPatton();
				}
				if (StartBeat + 13 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 14 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 15 >= BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
					attackPatton();
				}
				else if (StartBeat + 16 <= BossYsComponent->GetBeatCount())
				{
					Once = true;
				}
			}
			break;
			default:
				break;
			}
		}

		switch (mCurrentState)
		{
		case TestGeon::eBossState::Idle:
			break;
		case TestGeon::eBossState::Trace:
			break;
		case eBossState::AttackStandby:
			break;
		case TestGeon::eBossState::Attack:
			break;
		case TestGeon::eBossState::Death:
			break;
		}
	}

	void EveBossFSM::handleState(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
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
				bossEve->GetComponent<AnimationRenderer>()->SetIsActive(false);
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

		AnimationRenderer* YsAnimation = bossEve->GetComponent<AnimationRenderer>();
		EveBossComponent* YsAndEveComponent = bossEve->GetComponent<EveBossComponent>();

		switch (mCurrentState)
		{
		case eBossState::Idle:			// 가만히
		{
			YsAnimation->SetAnimationAsset(YsAndEveComponent->GetYsAnimation(L"EveIdle"));
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

			if (player->GetComponent<GridComponent>()->GetPosition().X >= bossEve->GetComponent<GridComponent>()->GetPosition().X)
				YsAnimation->SetIsLeft(true);
			else
				YsAnimation->SetIsLeft(false);
		}
		break;
		case eBossState::Trace:		// y좌표 추격
		{
			YsAnimation->SetAnimationAsset(YsAndEveComponent->GetYsAnimation(L"EveMove"));
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

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

			GameObject* Warning = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::mTempId++, static_cast<float>(mBeatManager->GetIntervalTime()));
			AnimationRenderer* sprite = Warning->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::mTempId++);
			GridComponent* grid = Warning->CreateComponent<GridComponent>(SoulBeaterProcessor::mTempId++);
			Transform* transform = Warning->CreateComponent<Transform>(SoulBeaterProcessor::mTempId++);

			Warning->SetObjectType(eObjectType::Item);
			sprite->SetSize({ 2000, 500 });
			sprite->SetTransformLayer(eTransformLayer::Grid);
			sprite->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"Warning"));

			grid->SetMaxX(playergrid->GetMaxX());
			grid->SetMaxY(playergrid->GetMaxY());
			grid->SetCellDistance(playergrid->GetCellDistance());
			grid->SetPosition({ playergrid->GetMaxX() / 2, playergrid->GetPosition().Y });

			mPrevPlayerY = playergrid->GetPosition().Y;
		}
		break;
		case eBossState::Attack:
		{
			YsAnimation->SetAnimationAsset(YsAndEveComponent->GetYsAnimation(L"EveAttack"));
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

			mbIsAttackComplete = true;

		}
		break;
		case eBossState::BeAttacked:
		{
			// 피격 당할시 랜덤 위치로 이동
			YsAnimation->SetAnimationAsset(YsAndEveComponent->GetYsAnimation(L"EveBeAttacked"));
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(false);
		}
		break;
		case eBossState::Death:
			YsAnimation->SetAnimationAsset(YsAndEveComponent->GetYsAnimation(L"EveDeath"));
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(false);
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
				mNextPhase = ePhase::Ending;
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
			unsigned int random1 = rand() % (MAX_X - 2) + 1;
			unsigned int random2 = rand() % (MAX_Y - 2) + 1;
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random1, random2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			break;
		}
		case ePhase::Two:
		{
			unsigned int random1 = rand() % (MAX_X - 2) + 1;
			unsigned int random2 = rand() % (MAX_Y - 2) + 1;
			unsigned int random3 = rand() % (MAX_X - 2) + 1;
			unsigned int random4 = rand() % (MAX_Y - 2) + 1;

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random1,random2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random3,random4 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			mNextState = eBossState::BeAttacked;

			break;
		}
		break;
		case ePhase::Three:
		{
			unsigned int random1 = rand() % (MAX_X - 2) + 1;
			unsigned int random2 = rand() % (MAX_Y - 2) + 1;

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random1, random2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			mNextState = eBossState::BeAttacked;

			break;
		}
		case ePhase::Four:
		{
			unsigned int random1 = rand() % (MAX_X - 2) + 1;
			unsigned int random2 = rand() % (MAX_Y - 2) + 1;
			unsigned int random3 = rand() % (MAX_X - 2) + 1;
			unsigned int random4 = rand() % (MAX_Y - 2) + 1;

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random1,random2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random3,random4 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			mNextState = eBossState::BeAttacked;

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
		grid->SetCellDistance(GRID_CELL_DISTANCE);
		sprite->SetSpriteType(eSpriteType::Sprite);
		sprite->SetSize({ (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
		sprite->SetUVRectangle({ 0,0,600,600 });
		sprite->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Blade"));
		sprite->SetIsLeft(true);
		sprite->SetTransformLayer(eTransformLayer::Grid);
		daggerComponent->SetBeatManager(mBeatManager);

		gridmove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		collider->SetRadius(50.f);
		collider->SetIsTrigger(true);
		grid->SetPosition({ MAX_X - 1, mPrevPlayerY });
	}

	d2dFramework::GameObject* EveBossFSM::createMonster()
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		Transform* transform = nullptr;
		AnimationRenderer* renderer = nullptr;
		GridComponent* grid = nullptr;
		GridMoveComponent* gridmove = nullptr;
		BaseMonsterComponent* baseMonsterComponent = nullptr;
		CircleCollider* collider = nullptr;

		monster = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::mTempId++);
		monster->SetObjectType(eObjectType::Enemy);
		transform = monster->CreateComponent<Transform>(SoulBeaterProcessor::mTempId++);
		renderer = monster->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::mTempId++);
		renderer->SetTransformLayer(eTransformLayer::Grid);
		grid = monster->CreateComponent<GridComponent>(SoulBeaterProcessor::mTempId++);
		gridmove = monster->CreateComponent<GridMoveComponent>(SoulBeaterProcessor::mTempId++);
		baseMonsterComponent = monster->CreateComponent<BaseMonsterComponent>(SoulBeaterProcessor::mTempId++);
		collider = monster->CreateComponent<CircleCollider>(SoulBeaterProcessor::mTempId++);

		renderer->SetSize({ 50,	50 });
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"EveDeath"));
		renderer->SetIsLoop(true);
		renderer->SetAnimationIndex(0);

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		assert(bossEve != nullptr);
		GridComponent* gridComponent = bossEve->GetComponent<GridComponent>();

		transform->SetTranslate({ 0, 0 });
		grid->SetCellDistance(gridComponent->GetCellDistance());
		grid->SetMaxX(gridComponent->GetMaxX());
		grid->SetMaxY(gridComponent->GetMaxY());
		gridmove->SetCurrentSpeed(0.5f);

		baseMonsterComponent->SetDetectionArea(3);
		baseMonsterComponent->SetAttackArea(0);
		baseMonsterComponent->SetMoveableBeatCount(1);

		grid->SetPosition({ 0,0 });
		collider->SetRadius(25);

		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterMonster", std::to_string(monster->GetId()));

		return monster;
	}

	void EveBossFSM::DebugRender(const D2D1::Matrix3x2F& camaraTransform)
	{

	}

	void EveBossFSM::Release()
	{

	}
}

