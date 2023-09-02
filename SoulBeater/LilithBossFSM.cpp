#include "LilithBossFSM.h"

#include "SoulBeaterProcessor.h"
#include "IncludeComponent.h"
#include "IncludeManager.h"

#include "BaseMonsterComponent.h"
#include "LilithBossComponent.h"
#include "BoomFlyComponent.h"

#include "MonsterFSM.h"
#include "ConstantTable.h"
#include "ObjectHelper.h"

#include "eSoulBeaterID.h"

#include <cassert>

namespace soulBeater
{
	LilithBossFSM::LilithBossFSM()
		:BaseEntity(static_cast<unsigned int>(eSoulBeaterID::LilithFSM))
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

	void LilithBossFSM::Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int bossId)
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
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));

		if (player == nullptr)
		{
			return;
		}
		if (boss == nullptr)
		{
			mNextPhase = ePhase::None;
			mCurrentPhase = ePhase::None;
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

		static unsigned int CurrentBeat = 0;
		static unsigned int NextBeat = 0;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossLilithId);

		LilithBossComponent* BossLilithComponent = boss->GetComponent<LilithBossComponent>();
		assert(BossLilithComponent != nullptr);

		BossLilithComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossLilithComponent->GetAccumulateCount(static_cast<float>(mBeatManager->GetIntervalTime()));

		// 액션 턴과 한 박자 쉬는 턴으로 구분
		if (CurrentBeat + mDelayBeat != NextBeat)
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

			static unsigned int StartBeat = 0;
			static bool IsOnce = true;

			if (IsOnce)
			{
				IsOnce = false;
				StartBeat = mBeatCount;
			}

			if (mCurrentPhase == ePhase::One)
			{
				if (mBeatCount % 3 == 0)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else
				{
					mNextState = eBossState::Trace;
				}
			}
			else if (mCurrentPhase == ePhase::Two)
			{
				if (StartBeat == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 1 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 2 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else if (StartBeat + 3 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 4 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 6 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 7 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(2);
				}
				else if (StartBeat + 8 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(2);
				}
				else if (StartBeat + 9 <= mBeatCount)
				{
					IsOnce = true;
				}
			}
			else if (mCurrentPhase == ePhase::Three)
			{
				if (StartBeat == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 1 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 2 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else if (StartBeat + 3 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 4 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 6 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 7 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(2);
				}
				else if (StartBeat + 8 == mBeatCount)
				{
					mNextState = eBossState::Trace;
					attackPatton(2);
				}
				else if (StartBeat + 9 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 10 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 11 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(1);
				}
				else if (StartBeat + 12 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 13 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					attackPatton(2);
				}
				else if (StartBeat + 14 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 15 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 16 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				else if (StartBeat + 17 <= mBeatCount)
				{
					IsOnce = true;
				}
			}
			else if (mCurrentPhase == ePhase::Four || mCurrentPhase == ePhase::Ending)
			{
				StartBeat = 0;
				CurrentBeat = 0;
				IsOnce = true;
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
		}
		break;
		case eBossState::Trace:
		{
			GridMoveComponent* girdmove = boss->GetComponent<GridMoveComponent>();

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
			AnimationRenderer* animation = boss->GetComponent<AnimationRenderer>();
			
			if (animation->GetIsAnimationEnd())
			{
				mBossTurn = eBossTurn::StayTurn;
				mNextState = eBossState::Idle;
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
				ObjectManager::GetInstance()->DeleteObject(mBossLilithId);
				mNextState = eBossState::Idle;
				mCurrentState = eBossState::Idle;
				mNextPhase = ePhase::None;
				mCurrentPhase = ePhase::None;
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
			LilithAnimation->SetIsLoop(false);
		}
		break;
		case eBossState::BeAttacked:
		{
			SoundManager::GetInstance()->Play(EFFECT_LILITHHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 5);

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
			ObjectHelper::CreateMonster({ 3, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 5 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 5, 2 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 5 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 7, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 9, 6 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 2 }, eMonsterType::Hold);

			break;
		}
		case ePhase::Two:
		{
			ObjectHelper::CreateMonster({ 2, 2 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 6 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 5, 2 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 7, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 8, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 9, 2 }, eMonsterType::Hold);

			mNextState = eBossState::BeAttacked;

			break;
		}
		break;
		case ePhase::Three:
		{
			ObjectHelper::CreateMonster({ 2, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 6 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 5, 1 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 6, 2 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 7, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 8, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 10, 2 }, eMonsterType::Escape);

			mNextState = eBossState::BeAttacked;

			break;
		}
		case ePhase::Four:
		{
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


			boomFlyComponent->SetCellCountToExplosion(-direction.X);
			direction.X = -1;
			direction.Y = 0;

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

			direction.X = -1;
			direction.Y = 0;

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
		gridMove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		return projectile;
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