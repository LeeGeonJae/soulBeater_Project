#include "YsBossFSM.h"

#include "BeatManager.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "EventManager.h"

#include "GameObject.h"
#include "CircleCollider.h"
#include "YsBossComponent.h"
#include "GridMoveComponent.h"
#include "GridComponent.h"
#include "AnimationRenderer.h"
#include "SpriteRenderer.h"
#include "DaggerComponent.h"
#include "BaseMonsterComponent.h"
#include "EveBossComponent.h"
#include "SoulBeaterProcessor.h"

#include "MonsterFSM.h"
#include "ObjectHelper.h"

#include "eSoulBeaterID.h"

#include <random>

#include "SoundManager.h"

namespace soulBeater
{
	YsBossFSM::YsBossFSM()
		: d2dFramework::BaseEntity(static_cast<unsigned int>(eSoulBeaterID::YsFSM))
		, mBeatManager(nullptr)
		, mYsID(static_cast<unsigned>(-1))
		, mEveID(static_cast<unsigned>(-1))
		, mPlayerID(static_cast<unsigned>(-1))
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

	YsBossFSM::~YsBossFSM()
	{

	}

	void YsBossFSM::Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int ysID, unsigned int eveID, unsigned int playerID)
	{
		using namespace d2dFramework;
		srand(static_cast<unsigned int>(time(nullptr)));

		mBeatManager = beatmanager;
		mMonsterFSM = monsterFSM;
		mYsID = ysID;
		mEveID = eveID;
		mPlayerID = playerID;
	}

	void YsBossFSM::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);

		if (bossEve == nullptr || bossYs == nullptr)
		{
			mNextPhase = ePhase::None;
			mCurrentPhase = ePhase::None;
			
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

	void YsBossFSM::update(float deltaTime)
	{
		using namespace d2dFramework;

		static unsigned int CurrentBeat = 0;
		static unsigned int NextBeat = 0;

		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		assert(bossYs != nullptr);
		YsBossComponent* BossYsComponent = bossYs->GetComponent<YsBossComponent>();
		assert(BossYsComponent != nullptr);

		BossYsComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossYsComponent->GetAccumulateCount(static_cast<float>(mBeatManager->GetIntervalTime()));

		// 액션 턴과 한 박자 쉬는 턴으로 구분
		if (CurrentBeat + mDelayBeat != NextBeat)
		{
			BossYsComponent->AddBeatCount(1);

			CurrentBeat = NextBeat;

			if ((int)mBossTurn <= 2)
			{
				mBossTurn = (eBossTurn)((int)mBossTurn + 1);
			}
			else
			{
				mBossTurn = eBossTurn::StayTurn;
			}

			static unsigned int StartBeat = 0;
			static bool bIsOnce = true;

			if (bIsOnce)
			{
				bIsOnce = false;
				StartBeat = BossYsComponent->GetBeatCount();
			}

			switch (mCurrentPhase)
			{
			case ePhase::None:
				break;
			case ePhase::One:
			{
				if (StartBeat == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 1 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 2 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 3 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 4 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
				}
				else if (StartBeat + 5 != BossYsComponent->GetBeatCount())
				{
					bIsOnce = true;
				}
			}
			break;
			case ePhase::Two:
			{
				if (StartBeat == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 1 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 2 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 3 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 4 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
				}
				else if (StartBeat + 5 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 6 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 7 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 8 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
				}
				else if (StartBeat + 9 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 10 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 12 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 12 != BossYsComponent->GetBeatCount())
				{
					bIsOnce = true;
				}
			}
			break;
			case ePhase::Three:
			{
				if (StartBeat == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 1 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 2 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 3 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 4 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
				}
				else if (StartBeat + 5 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 6 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 7 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 8 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 9 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 12 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::AttackStandby;
				}
				else if (StartBeat + 13 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Attack;
				}
				else if (StartBeat + 14 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Idle;
				}
				else if (StartBeat + 15 == BossYsComponent->GetBeatCount())
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 16 != BossYsComponent->GetBeatCount())
				{
					bIsOnce = true;
				}
			}
			break;
			case ePhase::Four:
				StartBeat = 0;
				CurrentBeat = 0;
				bIsOnce = true;
				break;
			case ePhase::Ending:
				StartBeat = 0;
				CurrentBeat = 0;
				bIsOnce = true;
				break;
			default:
				break;
			}
		}
	}

	void YsBossFSM::handleState(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		assert(player != nullptr);
		assert(bossYs != nullptr);

		switch (mCurrentState)
		{
		case eBossState::Idle:
		{
			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = bossYs->GetComponent<GridComponent>();
		}
		break;
		case eBossState::Trace:
		{
			GridMoveComponent* girdmove = bossYs->GetComponent<GridMoveComponent>();

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
			GridMoveComponent* girdmove = bossYs->GetComponent<GridMoveComponent>();

			if (girdmove->GetIsCanMove())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::BeAttacked:
		{
			AnimationRenderer* animation = bossYs->GetComponent<AnimationRenderer>();

			if (animation->GetIsAnimationEnd())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::Death:
		{
			
		}
		break;
		}
	}

	void YsBossFSM::enter()
	{
		using namespace d2dFramework;

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(mPlayerID);
		GameObject* bossYs = ObjectManager::GetInstance()->FindObjectOrNull(mYsID);
		GameObject* bossEve = ObjectManager::GetInstance()->FindObjectOrNull(mEveID);
		assert(player != nullptr);
		assert(bossYs != nullptr);
		assert(bossEve != nullptr);

		AnimationRenderer* YsAnimation = bossYs->GetComponent<AnimationRenderer>();
		YsBossComponent* YsAndEveComponent = bossYs->GetComponent<YsBossComponent>();

		switch (mCurrentState)
		{
		case eBossState::Idle:			// 가만히
		{
			YsAnimation->SetAnimationIndex(1);
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

			if (player->GetComponent<GridComponent>()->GetPosition().X >= bossYs->GetComponent<GridComponent>()->GetPosition().X)
			{
				YsAnimation->SetIsLeft(true);
			}
			else
			{
				YsAnimation->SetIsLeft(false);
			}
		}
		break;
		case eBossState::Trace:		// y좌표 추격
		{
			YsAnimation->SetAnimationIndex(0);
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = bossYs->GetComponent<GridComponent>();
			GridMoveComponent* MovegridComponent = bossEve->GetComponent<GridMoveComponent>();

			// 플레이어가 보스보다 왼쪽에 있을 경우 다른 보스와 같은 X위치에 없을 때
			if (playergridcomponent->GetPosition().Y > bossgridcomponent->GetPosition().Y
				&& MovegridComponent->GetCurrentPosition().X != bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossYs->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, 1);
			}
			// 같은 X 좌표일 때, 다른 보스의 다음 Y값 계산해서 이동
			else if (playergridcomponent->GetPosition().Y > bossgridcomponent->GetPosition().Y
				&& MovegridComponent->GetNextPosition().Y != bossgridcomponent->GetPosition().Y + 1
				&& MovegridComponent->GetCurrentPosition().Y != bossgridcomponent->GetPosition().Y + 1
				&& MovegridComponent->GetCurrentPosition().X == bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossYs->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, 1);
			}

			// 플레이어가 보스보다 왼쪽에 있을 경우 다른 보스와 같은 X위치에 없을 때
			if (playergridcomponent->GetPosition().Y < bossgridcomponent->GetPosition().Y
				&& MovegridComponent->GetCurrentPosition().X != bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossYs->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, -1);
			}
			// 같은 X 좌표일 때, 다른 보스의 다음 Y값 계산해서 이동
			else if (playergridcomponent->GetPosition().Y < bossgridcomponent->GetPosition().Y
				&& MovegridComponent->GetNextPosition().Y != bossgridcomponent->GetPosition().Y - 1
				&& MovegridComponent->GetCurrentPosition().Y != bossgridcomponent->GetPosition().Y - 1
				&& MovegridComponent->GetCurrentPosition().X == bossgridcomponent->GetPosition().X)
			{
				GridMoveComponent* bossgridmovecomponent = bossYs->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, -1);
			}
		}
		break;
		case eBossState::AttackStandby:
		{
			GridComponent* playergrid = player->GetComponent<GridComponent>();
			GridComponent* bossgrid = bossYs->GetComponent<GridComponent>();
			GridMoveComponent* bossmovegrid = bossYs->GetComponent<GridMoveComponent>();

			GameObject* object = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::GetTempId(), 0.5f);
			object->SetObjectType(eObjectType::Item);

			GridComponent* grid = object->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
			Transform* transform = object->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
			AnimationRenderer* sprite = object->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());

			grid->SetMaxX(playergrid->GetMaxX());
			grid->SetMaxY(playergrid->GetMaxY());
			grid->SetCellDistance(playergrid->GetCellDistance());
			grid->SetPosition({ playergrid->GetPosition().X, bossmovegrid->GetNextPosition().Y });

			sprite->SetSize({ 3000, 700 });
			sprite->SetTransformLayer(eTransformLayer::Grid);
			sprite->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(WARNING_ANI_KEY));
		}
		break;
		case eBossState::Attack:
		{
			YsAnimation->SetAnimationIndex(2);
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(true);

			// 공격 구현 ( 둘 중 하나 랜덤 ? )
			// 플레이어 방향으로 한 박자 안에 벽 까지 직진
			// 한 박자에 한 칸 씩 이동하는 검 1개 발사
			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = bossYs->GetComponent<GridComponent>();
			GridComponent* eveBossgridcomponent = bossEve->GetComponent<GridComponent>();
			GridMoveComponent* evegridmovecomponent = bossEve->GetComponent<GridMoveComponent>();
			GridMoveComponent* movegridcomponent = bossYs->GetComponent<GridMoveComponent>();

			if (playergridcomponent->GetPosition().X > bossgridcomponent->GetPosition().X)
			{
				movegridcomponent->Move(10, 0);
			}
			else if (playergridcomponent->GetPosition().X < bossgridcomponent->GetPosition().X)
			{
				movegridcomponent->Move(-10, 0);
			}

			mbIsAttackComplete = true;
		}
		break;
		case eBossState::BeAttacked:
		{
			SoundManager::GetInstance()->Play(EFFECT_YSHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 9);

			YsAnimation->SetAnimationIndex(3);
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(false);
		}
		break;
		case eBossState::Death:
		{
			YsAnimation->SetAnimationIndex(4);
			YsAnimation->SetFrameindex(0);
			YsAnimation->SetIsLoop(false);
		}
		break;
		}
	}

	void YsBossFSM::exit()
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

	void YsBossFSM::handlePhaseState()
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

	void YsBossFSM::enterPhase()
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

			break;
		}
		case ePhase::Two:
		{

			mNextState = eBossState::BeAttacked;

			break;
		}
		break;
		case ePhase::Three:
		{

			mNextState = eBossState::BeAttacked;

			break;
		}
		case ePhase::Four:
		{
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

	void YsBossFSM::DebugRender(const D2D1::Matrix3x2F& camaraTransform)
	{

	}

	void YsBossFSM::Release()
	{

	}

}

