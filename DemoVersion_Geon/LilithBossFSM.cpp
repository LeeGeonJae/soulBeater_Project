#include "LilithBossFSM.h"

#include "SoulBeaterProcessor.h"
#include "IncludeComponent.h"
#include "IncludeManager.h"

#include "BaseMonsterComponent.h"
#include "LilithBossComponent.h"
#include "BoomFlyComponent.h"

#include "MonsterFSM.h"

#include <cassert>

namespace TestGeon
{
	LilithBossFSM::LilithBossFSM()
		:BaseEntity(90005)
		, mBeatManager(nullptr)
		, mBossLilithId(0)
		, mCurrentState(eBossState::Idle)
		, mNextState(eBossState::Idle)
		, mBossTurn(eBossTurn::StayTurn)
		, mbIsAttackComplete(false)
		, mDelayBeat(0)
		, mCurrentPhase(ePhase::None)
		, mNextPhase(ePhase::None)
		, mMonsterFSM(nullptr)
		, mBeatCount(0)
	{
	}

	LilithBossFSM::~LilithBossFSM()
	{
		mBeatManager = nullptr;
	}

	void LilithBossFSM::Init(d2dFramework::BeatManager* beatmanager, unsigned int bossId, MonsterFSM* monsterFSM)
	{
		assert(beatmanager != nullptr);

		mBeatManager = beatmanager;
		mBossLilithId = bossId;
		mMonsterFSM = monsterFSM;

		mCurrentState = eBossState::Trace;
		mCurrentPhase = ePhase::None;
	}

	void LilithBossFSM::Update(float deltaTime)
	{

		assert(mBossLilithId != static_cast<unsigned int>(-1));
		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		if (boss == nullptr)
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

	void LilithBossFSM::Release()
	{

	}

	void LilithBossFSM::update(float deltaTime)
	{
		using namespace d2dFramework;

		static int CurrentBeat = 0;
		static int NextBeat = 0;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		LilithBossComponent* BossLilithComponent = boss->GetComponent<LilithBossComponent>();
		assert(BossLilithComponent != nullptr);

		BossLilithComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossLilithComponent->GetAccumulateCount(mBeatManager->GetIntervalTime());

		// 액션 턴과 한 박자 쉬는 턴으로 구분
		if (CurrentBeat + mDelayBeat < NextBeat)
		{
			mBeatCount++;
			CurrentBeat = NextBeat;

			if ((int)mBossTurn <= 2)
			{
				mBossTurn = (eBossTurn)((int)mBossTurn + 1);
			}
			else
			{
				mBossTurn = eBossTurn::StayTurn;
			}

			// 공격
			if (mCurrentState == eBossState::Attack)
			{
				static int StartBeat = 0;
				static bool IsOnce = true;

				if (IsOnce)
				{
					IsOnce = false;
					StartBeat = mBeatCount;
				}

				if (mCurrentPhase == ePhase::One)
				{
					attackPatton(1);
					IsOnce = true;
				}
				else if (mCurrentPhase == ePhase::Two)
				{
					if (StartBeat == mBeatCount)
					{
						attackPatton(1);
					}
					if (StartBeat + 1 == mBeatCount)
					{
						attackPatton(1);
					}
					if (StartBeat + 3 == mBeatCount)
					{
						attackPatton(2);
					}
					if (StartBeat + 4 <= mBeatCount)
					{
						IsOnce = true;
					}
				}
				else if (mCurrentPhase == ePhase::Three)
				{
					if (StartBeat == mBeatCount)
					{
						attackPatton(2);
					}
					if (StartBeat + 2 == mBeatCount)
					{
						attackPatton(1);
					}
					if (StartBeat + 3 == mBeatCount)
					{
						attackPatton(2);
					}
					if (StartBeat + 4 <= mBeatCount)
					{
						IsOnce = true;
					}
				}
			}
		}
	}

	void LilithBossFSM::handleState(float deltaTime)
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);
		GameObject* player = getPlayer();

		switch (mCurrentState)
		{
		case eBossState::Idle:
		{
			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = boss->GetComponent<GridComponent>();

			// 플레이어와 위치가 똑같으면 전투 상태로 전환
			if (mBossTurn == eBossTurn::ActionStandbyTurn && playergridcomponent->GetPosition().Y == bossgridcomponent->GetPosition().Y)
			{
				mNextState = eBossState::AttackStandby;
			}
			else if (mBossTurn == eBossTurn::ActionTurn)
			{
				mNextState = eBossState::Trace;
			}
		}
		break;
		case eBossState::Trace:
		{
			GridMoveComponent* girdmove = boss->GetComponent<GridMoveComponent>();

			if (mBossTurn == eBossTurn::StayTurn && girdmove->GetIsCanMove())
			{
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::AttackStandby:
		{
			if (mBossTurn == eBossTurn::ActionTurn)
			{
				mNextState = eBossState::Attack;
			}
		}
		break;
		case eBossState::Attack:
		{
			GridMoveComponent* girdmove = boss->GetComponent<GridMoveComponent>();

			static int StartBeat = 0;
			static bool IsOnce = true;

			if (IsOnce)
			{
				IsOnce = false;
				StartBeat = mBeatCount;
			}

			if (girdmove->GetIsCanMove() && mCurrentPhase == ePhase::One && StartBeat + 1 <= mBeatCount)
			{
				IsOnce = true;
				mNextState = eBossState::Idle;
				mBossTurn = eBossTurn::StayTurn;
			}
			if (girdmove->GetIsCanMove() && mCurrentPhase == ePhase::Two && StartBeat + 4 <= mBeatCount)
			{
				IsOnce = true;
				mNextState = eBossState::Idle;
				mBossTurn = eBossTurn::StayTurn;
			}
			if (girdmove->GetIsCanMove() && mCurrentPhase == ePhase::Three && StartBeat + 5 <= mBeatCount)
			{
				IsOnce = true;
				mNextState = eBossState::Idle;
				mBossTurn = eBossTurn::StayTurn;
			}
		}
		break;
		case eBossState::BeAttacked:
		{
			AnimationRenderer* animation = boss->GetComponent<AnimationRenderer>();

			if (animation->GetIsAnimationEnd())
			{
				mBossTurn = eBossTurn::StayTurn;
				mNextState = eBossState::Idle;
			}
		}
		break;
		case eBossState::Death:
		{
			boss->GetComponent<CircleCollider>()->SetIsTrigger(false);

			if (boss->GetComponent<AnimationRenderer>()->GetIsAnimationEnd())
			{
				boss->GetComponent<AnimationRenderer>()->SetIsActive(false);
			}
		}
		break;
		}
	}

	void LilithBossFSM::enter()
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		AnimationRenderer* LilithAnimation = boss->GetComponent<AnimationRenderer>();
		LilithBossComponent* BossComponent = boss->GetComponent<LilithBossComponent>();

		assert(boss != nullptr);

		GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
		assert(bossGridMoveComponent != nullptr);

		GameObject* player = getPlayer();
		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

		GridVector direction = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

		switch (mCurrentState)
		{
		case eBossState::Idle:			// 가만히
		{
			LilithAnimation->SetAnimationIndex(1);
			LilithAnimation->SetFrameindex(0);
			LilithAnimation->SetIsLoop(true);

			if (player->GetComponent<GridComponent>()->GetPosition().X >= boss->GetComponent<GridComponent>()->GetPosition().X)
			{
				LilithAnimation->SetIsLeft(true);
			}
			else
			{
				LilithAnimation->SetIsLeft(false);
			}
		}
		break;
		case eBossState::Trace:		// y좌표 추격
		{
			LilithAnimation->SetAnimationIndex(0);
			LilithAnimation->SetFrameindex(0);
			LilithAnimation->SetIsLoop(false);

			GridComponent* playergridcomponent = player->GetComponent<GridComponent>();
			GridComponent* bossgridcomponent = boss->GetComponent<GridComponent>();

			if (playergridcomponent->GetPosition().Y > bossgridcomponent->GetPosition().Y)
			{
				GridMoveComponent* bossgridmovecomponent = boss->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, 1);
			}

			if (playergridcomponent->GetPosition().Y < bossgridcomponent->GetPosition().Y)
			{
				GridMoveComponent* bossgridmovecomponent = boss->GetComponent<GridMoveComponent>();
				bossgridmovecomponent->Move(0, -1);
			}
		}
		break;
		case eBossState::AttackStandby:
		{
		}
		break;
		case eBossState::Attack:
		{
			LilithAnimation->SetAnimationIndex(2);
			LilithAnimation->SetFrameindex(0);
			LilithAnimation->SetIsLoop(true);
		}
		break;
		case eBossState::BeAttacked:
		{
			LilithAnimation->SetAnimationIndex(3);
			LilithAnimation->SetFrameindex(0);
			LilithAnimation->SetIsLoop(false);
		}
		break;
		case eBossState::Death:
		{
			LilithAnimation->SetAnimationIndex(4);
			LilithAnimation->SetFrameindex(0);
			LilithAnimation->SetIsLoop(true);
		}
		break;
		}
	}

	void LilithBossFSM::exit()
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

	void LilithBossFSM::handlePhaseState()
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
		case ePhase::Ending:
			// ending이 종료되면
			break;
		default:
			assert(false);
			break;
		}
	}

	void LilithBossFSM::enterPhase()
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

			break;
		}
		case ePhase::Two:
		{
			unsigned int random1 = rand() % (MAX_X - 2) + 1;
			unsigned int random2 = rand() % (MAX_Y - 2) + 1;

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ random1,random2 });
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
			grid->SetPosition({ random1,random2 });
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

	void LilithBossFSM::attackPatton(int pattonNumber)
	{
		GameObject* player = getPlayer();
		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

		GridVector direction = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

		switch (pattonNumber)
		{
		case 1:
		{
			GameObject* projectile = createProjectile();
			GridComponent* gridCompoent = projectile->GetComponent<GridComponent>();

			BoomFlyComponent* boomFlyComponent = projectile->CreateComponent<BoomFlyComponent>(mProjectileObjectId++);
			boomFlyComponent->SetStartPosition(bossGridComponent->GetPosition());

			if (direction.X == 0)
			{
				if (direction.Y > 0)
				{
					boomFlyComponent->SetCellCountToExplosion(direction.Y);

					direction.X = 0;
					direction.Y = 1;
				}
				else
				{
					boomFlyComponent->SetCellCountToExplosion(-direction.Y);
					direction.X = 0;
					direction.Y = -1;
				}
			}
			else
			{
				if (direction.X > 0)
				{
					boomFlyComponent->SetCellCountToExplosion(direction.X);
					direction.X = 1;
					direction.Y = 0;
				}
				else
				{
					boomFlyComponent->SetCellCountToExplosion(-direction.X);
					direction.X = -1;
					direction.Y = 0;
				}
			}
			boomFlyComponent->SetDirection(direction);
			boomFlyComponent->SetIsRepeat(false);
			projectile->Init();
		}
			break;
		case 2:
		{
			GameObject* projectile = createProjectile();
			GridComponent* gridCompoent = projectile->GetComponent<GridComponent>();

			BoomFlyComponent* boomFlyComponent = projectile->CreateComponent<BoomFlyComponent>(mProjectileObjectId++);
			boomFlyComponent->SetStartPosition(bossGridComponent->GetPosition());
			boomFlyComponent->SetCellCountToExplosion(2);
			if (direction.X == 0)
			{
				if (direction.Y > 0)
				{
					direction.Y = 1;
					direction.X = 0;
				}
				else
				{
					direction.Y = -1;
					direction.X = 0;
				}
			}
			else
			{
				if (direction.X > 0)
				{
					direction.X = 1;
					direction.Y = 0;
				}
				else
				{
					direction.X = -1;
					direction.Y = 0;
				}
			}
			boomFlyComponent->SetDirection(direction);
			boomFlyComponent->SetIsRepeat(true);
			projectile->Init();
		}
			break;
		}

	}

	d2dFramework::GameObject* LilithBossFSM::createProjectile()
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		mProjectileIds.insert(mProjectileObjectId);
		GameObject* projectile = ObjectManager::GetInstance()->CreateObject(mProjectileObjectId++);

		Transform* transform = projectile->CreateComponent<Transform>(mProjectileObjectId++);
		SpriteRenderer* renderer = projectile->CreateComponent<SpriteRenderer>(mProjectileObjectId++);
		GridComponent* grid = projectile->CreateComponent<GridComponent>(mProjectileObjectId++);
		GridMoveComponent* gridMove = projectile->CreateComponent<GridMoveComponent>(mProjectileObjectId++);
		CircleCollider* circleCollider = projectile->CreateComponent<CircleCollider>(mProjectileObjectId++);

		projectile->SetObjectType(eObjectType::Projectile);

		circleCollider->SetRadius(25);

		renderer->SetTransformLayer(eTransformLayer::Grid);
		renderer->SetSpriteType(eSpriteType::Sprite);
		renderer->SetUVRectangle({ 0,0,600,600 });
		renderer->SetIsActive(true);
		renderer->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Knife"));

		assert(boss != nullptr);
		GridComponent* bossGrid = boss->GetComponent<GridComponent>();

		renderer->SetSize({ static_cast<float>(bossGrid->GetCellDistance()), static_cast<float>(bossGrid->GetCellDistance()) });
		grid->SetCellDistance(bossGrid->GetCellDistance());
		grid->SetMaxX(bossGrid->GetMaxX());
		grid->SetMaxY(bossGrid->GetMaxY());
		grid->SetPosition(bossGrid->GetPosition());
		gridMove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		return projectile;
	}

	d2dFramework::GameObject* LilithBossFSM::createMonster()
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

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

		assert(boss != nullptr);
		GridComponent* gridComponent = boss->GetComponent<GridComponent>();

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

	d2dFramework::GameObject* LilithBossFSM::getPlayer()
	{
		using namespace d2dFramework;

		std::set<unsigned int> playerIds = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Player);
		assert(playerIds.size() == 1);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(*playerIds.begin());
		assert(player != nullptr);

		return player;
	}
}