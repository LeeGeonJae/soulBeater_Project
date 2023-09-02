#include "BealzebubFSM.h"

#include "SoulBeaterProcessor.h"

#include "ConstantTable.h"
#include "MonsterFSM.h"
#include "RenderManger.h"
#include "BeatManager.h"
#include "FlyComponent.h"
#include "GridPosition.h"
#include "GridVector.h"
#include "ObjectManager.h"
#include "EventManager.h"
#include "FastFlyComponent.h"
#include "BoomFlyComponent.h"
#include "IncludeComponent.h"
#include "BealzebubComponent.h"

#include "ObjectHelper.h"
#include "eObjectType.h"
#include "eSoulBeaterID.h"

#include <set>
#include <map>
#include <cassert>
#include "ConstantTable.h"
#include "SoundManager.h"

namespace soulBeater
{
	BealzebubFSM::BealzebubFSM()
		: BaseEntity(static_cast<unsigned int>(eSoulBeaterID::BealzebubBossFSM))
		, mBeatManager(nullptr)
		, mMonsterFSM(nullptr)
		, mBossId(static_cast<unsigned int>(-1))
		, mBeatCount()
		, mCurrentState()
		, mNextState()
		, mCurrentPhase()
		, mCurrentBealzebubAttack()
		, mbIsHit()
		, mbIsHitable()
		, mProjectileObjectId(20000012)
		, mProjectileIds()
		, mAttackDelayTime()
	{
	}
	BealzebubFSM::~BealzebubFSM()
	{
		mBeatManager = nullptr;
	}

	void BealzebubFSM::Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int bossId)
	{
		assert(beatmanager != nullptr);
		mBeatManager = beatmanager;
		mBossId = bossId;

		mCurrentState = eBossState::Trace;
		mCurrentPhase = ePhase::None;
		mCurrentBealzebubAttack = eBealzebubAttack::Fly;

		mMonsterFSM = monsterFSM;
	}

	void BealzebubFSM::Release()
	{
		mBeatManager = nullptr;
		mBossId = static_cast<unsigned int>(-1);
	}

	void BealzebubFSM::Update(float deltaTime)
	{
		using namespace d2dFramework;

		assert(mBossId != static_cast<unsigned int>(-1));

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));

		if (player == nullptr)
		{
			return;
		}
		if (boss == nullptr)
		{
			mCurrentPhase = ePhase::None;
			return;
		}

		mNextState = handleState();
		update(deltaTime);

		if (mCurrentState != mNextState)
		{
			exit();
			mCurrentState = mNextState;
			enter();
		}

		for (auto iter = mProjectileIds.begin(); iter != mProjectileIds.end();)
		{
			GameObject* projectile = ObjectManager::GetInstance()->FindObjectOrNull(*iter);

			if (projectile == nullptr)
			{
				iter = mProjectileIds.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		ePhase nextPhase = handlePhaseState();

		if (mCurrentPhase != nextPhase)
		{
			mCurrentPhase = nextPhase;
			enterPhase();
		}
	}

	void BealzebubFSM::DebugRender(const D2D1::Matrix3x2F& camaraTransform)
	{

	}

	eBossState BealzebubFSM::handleState()
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);

		GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
		assert(bossGridMoveComponent != nullptr);

		GameObject* player = getPlayer();
		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

		GridVector distance = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

		switch (mCurrentState)
		{
		case eBossState::Idle:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				if (mCurrentPhase == ePhase::Ending)
				{
					return eBossState::Death;
				}

				return eBossState::BeAttacked;
			}
			break;
		case eBossState::Trace:
		{
			// if Monster가 모두 죽으면 
			GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
			assert(bossGridMoveComponent != nullptr);

			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				if (mCurrentPhase == ePhase::Ending)
				{
					return eBossState::Death;
				}

				return eBossState::BeAttacked;
			}

			break;
		}
		case eBossState::Attack:
		{
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				if (mCurrentPhase == ePhase::Ending)
				{
					return eBossState::Death;
				}

				return eBossState::BeAttacked;
			}

			AnimationRenderer* renderer = boss->GetComponent<AnimationRenderer>();
			if (renderer->GetIsAnimationEnd())
			{
				return eBossState::Idle;
			}
		}
		break;
		case eBossState::BeAttacked:
		{
			AnimationRenderer* renderer = boss->GetComponent<AnimationRenderer>();
			if (renderer->GetIsAnimationEnd())
			{
				return eBossState::Idle;
			}

			break;
		}
		case eBossState::Death:
		{
			AnimationRenderer* renderer = boss->GetComponent<AnimationRenderer>();
			if (renderer->GetIsAnimationEnd())
			{
				mNextState = eBossState::Idle;
				mCurrentState = eBossState::Idle;
				mCurrentPhase = ePhase::None;
				ObjectManager::GetInstance()->DeleteObject(mBossId);
			}
			break;
		}
		default:
			assert(false);
		}

		return mCurrentState;
	}

	void BealzebubFSM::update(float deltaTime)
	{
		// 애니메이션 업데이트
		using namespace d2dFramework;

		GameObject* player = getPlayer();
		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(player != nullptr);
		assert(boss != nullptr);

		AnimationRenderer* bossAniComponent = boss->GetComponent<AnimationRenderer>();
		assert(bossAniComponent != nullptr);

		if (player->GetComponent<GridComponent>()->GetPosition().X >= boss->GetComponent<GridComponent>()->GetPosition().X)
		{
			bossAniComponent->SetIsLeft(true);
		}
		else
		{
			bossAniComponent->SetIsLeft(false);
		}

		static int CurrentBeat = 0;
		static int NextBeat = 0;

		BealzebubComponent* BossLilithComponent = boss->GetComponent<BealzebubComponent>();
		assert(BossLilithComponent != nullptr);

		BossLilithComponent->AccumulateBeatElapsedTime(deltaTime);
		NextBeat = BossLilithComponent->GetAccumulateCount(static_cast<float>(mBeatManager->GetIntervalTime()));

		if (CurrentBeat != NextBeat)
		{
			mBeatCount++;
			CurrentBeat = NextBeat;

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
					enterAttack(eBealzebubAttack::Fly);
				}
				else if (mBeatCount % 2 == 0)
				{
					mNextState = eBossState::Trace;
				}
				else
				{
					mNextState = eBossState::Idle;
				}
			}
			else if (mCurrentPhase == ePhase::Two)
			{
				if (StartBeat == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 1 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::Fly);
				}
				if (StartBeat + 2 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 3 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 4 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::Fly);
				}
				if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 6 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 7 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFly);
				}
				if (StartBeat + 8 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 9 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 10 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFly);
				}
				if (StartBeat + 11 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 12 <= mBeatCount)
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
				if (StartBeat + 1 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::Fly);
				}
				if (StartBeat + 2 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 3 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 4 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::Fly);
				}
				if (StartBeat + 5 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 6 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 7 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFly);
				}
				if (StartBeat + 8 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 9 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 10 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFly);
				}
				if (StartBeat + 11 == mBeatCount)
				{
					mNextState = eBossState::Trace;
				}
				if (StartBeat + 12 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFlyAll);
				}
				if (StartBeat + 13 == mBeatCount)
				{
					mNextState = eBossState::Attack;
					enterAttack(eBealzebubAttack::FastFlyAll);
				}
				if (StartBeat + 14 <= mBeatCount)
				{
					IsOnce = true;
				}
			}
			else if (mCurrentPhase == ePhase::Four || mCurrentPhase == ePhase::Ending)
			{
				CurrentBeat = 0;
				StartBeat = 0;
				IsOnce = true;
			}
		}

		switch (mCurrentState)
		{
		case eBossState::Idle:
			break;
		case eBossState::Trace:
			updateMove();
			break;
		case eBossState::Attack:
			break;
		case eBossState::BeAttacked:
			break;
		case eBossState::Death:
			break;
		default:
			assert(false);
		}
	}

	void BealzebubFSM::updateMove()
	{
		using namespace d2dFramework;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
		assert(bossGridMoveComponent != nullptr);

		if (!bossGridMoveComponent->GetIsCanMove())
		{
			return;
		}

		// 이동 속도 제한이 있으면 해당 처리도 해주어야 함

		GameObject* player = getPlayer();
		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

		GridVector distance = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

		if (distance.X == 0 || distance.Y == 0)
		{
			return;
		}
		if (abs(distance.X) < abs(distance.Y))
		{
			bossGridMoveComponent->Move(distance.X > 0 ? 1 : -1, 0);
		}
		else
		{
			bossGridMoveComponent->Move(0, distance.Y > 0 ? 1 : -1);
		}
	}

	void BealzebubFSM::enter()
	{
		// 애니메이션 인덱스 변경
		using namespace d2dFramework;

		GameObject* player = getPlayer();
		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(boss != nullptr);
		AnimationRenderer* bossAniComponent = boss->GetComponent<AnimationRenderer>();
		GridMoveComponent* bossMove = boss->GetComponent<GridMoveComponent>();
		GridComponent* playergrid = player->GetComponent<GridComponent>();
		bossAniComponent->SetFrameindex(0);

		switch (mCurrentState)
		{
		case eBossState::Idle:
			bossAniComponent->SetAnimationIndex(0);
			bossAniComponent->SetIsLoop(true);
			break;
		case eBossState::Trace:
		{
			bossAniComponent->SetAnimationIndex(1);
			bossAniComponent->SetIsLeft(true);

			if (playergrid->GetPosition().Y > bossMove->GetCurrentPosition().Y)
			{
				bossMove->Move(0, 1);
			}
			else if (playergrid->GetPosition().Y < bossMove->GetCurrentPosition().Y)
			{
				bossMove->Move(0, -1);
			}
		}
		break;
		case eBossState::Attack:
			bossAniComponent->SetAnimationIndex(2);
			bossAniComponent->SetFrameindex(0);
			bossAniComponent->SetIsLoop(false);
			// 공격 오브젝트 생성
			break;
		case eBossState::BeAttacked:
			SoundManager::GetInstance()->Play(EFFECT_BEELZEBUBHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 3);
			bossAniComponent->SetAnimationIndex(3);
			bossAniComponent->SetIsLoop(false);
			// 피격 효과 생성 이벤트 던지기
			break;
		case eBossState::Death:
			bossAniComponent->SetAnimationIndex(4);
			bossAniComponent->SetIsLoop(false);
			// 죽음 효과 생성 이벤트 던지기
			break;
		default:
			assert(false);
		}
	}

	void BealzebubFSM::enterAttack(eBealzebubAttack attackType)
	{
		using namespace d2dFramework;

		mAttackDelayTime += 1.f;

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(boss != nullptr);

		GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
		assert(bossGridMoveComponent != nullptr);

		GameObject* player = getPlayer();
		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

		GridVector direction = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

		switch (attackType)
		{
		case eBealzebubAttack::Fly:
		{
			GameObject* projectiles[3];
			projectiles[0] = createProjectile();
			projectiles[1] = createProjectile();
			projectiles[2] = createProjectile();
			GridComponent* gridCompoents[3];
			gridCompoents[0] = projectiles[0]->GetComponent<GridComponent>();
			gridCompoents[1] = projectiles[1]->GetComponent<GridComponent>();
			gridCompoents[2] = projectiles[2]->GetComponent<GridComponent>();

			GridPosition position = gridCompoents[1]->GetPosition();

			gridCompoents[0]->SetPosition({ position.X, position.Y - 1 });
			gridCompoents[2]->SetPosition({ position.X, position.Y + 1 });
			gridCompoents[1]->SetPosition({ position.X - 1, position.Y });

			FlyComponent* flyComponent = projectiles[0]->CreateComponent<FlyComponent>(mProjectileObjectId++);
			flyComponent = projectiles[1]->CreateComponent<FlyComponent>(mProjectileObjectId++);
			flyComponent = projectiles[2]->CreateComponent<FlyComponent>(mProjectileObjectId++);

			projectiles[0]->Init();
			projectiles[1]->Init();
			projectiles[2]->Init();

			break;
		}
		case eBealzebubAttack::FastFly:
		{
			GameObject* projectile = createProjectile();
			GridComponent* projectileGridComponent = projectile->GetComponent<GridComponent>();
			FastFlyComponent* fastflyCompoent = projectile->CreateComponent<FastFlyComponent>(mProjectileObjectId++);

			GameObject* player = getPlayer();
			GridComponent* playergrid = player->GetComponent<GridComponent>();

			fastflyCompoent->SetWaringDuration(static_cast<float>(mBeatManager->GetIntervalTime()));
			fastflyCompoent->SetStartPosition(bossGridComponent->GetPosition());

			projectileGridComponent->SetPosition({ projectileGridComponent->GetMaxX() - 1 , playergrid->GetPosition().Y });
			fastflyCompoent->SetIsColumn(true);

			projectile->Init();

			DrawWarning(true, playergrid->GetPosition().Y);

			break;
		}
		case eBealzebubAttack::FastFlyAll:
		{
			static int even = 0;

			even++;

			if (even % 2 == 0)
			{
				GameObject* player = getPlayer();
				GridComponent* playergrid = player->GetComponent<GridComponent>();

				for (unsigned int i = 0; i < playergrid->GetMaxY(); i += 2)
				{
					GameObject* projectile = createProjectile();
					GridComponent* projectileGridComponent = projectile->GetComponent<GridComponent>();
					FastFlyComponent* fastflyCompoent = projectile->CreateComponent<FastFlyComponent>(mProjectileObjectId++);

					fastflyCompoent->SetWaringDuration(static_cast<float>(mBeatManager->GetIntervalTime()));
					fastflyCompoent->SetStartPosition(bossGridComponent->GetPosition());

					projectileGridComponent->SetPosition({ projectileGridComponent->GetMaxX() - 1 , static_cast<unsigned int>(i) });
					fastflyCompoent->SetIsColumn(true);

					projectile->Init();

					DrawWarning(true, projectileGridComponent->GetPosition().Y);
				}
			}
			else
			{
				GameObject* player = getPlayer();
				GridComponent* playergrid = player->GetComponent<GridComponent>();

				for (unsigned int i = 1u; i < playergrid->GetMaxY(); i += 2u)
				{
					GameObject* projectile = createProjectile();
					GridComponent* projectileGridComponent = projectile->GetComponent<GridComponent>();
					FastFlyComponent* fastflyCompoent = projectile->CreateComponent<FastFlyComponent>(mProjectileObjectId++);

					fastflyCompoent->SetWaringDuration(static_cast<float>(mBeatManager->GetIntervalTime()));
					fastflyCompoent->SetStartPosition(bossGridComponent->GetPosition());

					projectileGridComponent->SetPosition({ projectileGridComponent->GetMaxX() - 1 , static_cast<unsigned int>(i) });
					fastflyCompoent->SetIsColumn(true);

					projectile->Init();

					DrawWarning(true, projectileGridComponent->GetPosition().Y);
				}
			}

			break;
		}
		default:
			break;
		}
	}

	void BealzebubFSM::exit()
	{
		switch (mCurrentState)
		{
		case eBossState::Idle:
			break;
		case eBossState::Trace:
			break;
		case eBossState::Attack:
			break;
		case eBossState::BeAttacked:
			// 피격 효과 종료 이벤트 던지기
			break;
		case eBossState::Death:
			// 죽음 효과 종료 이벤트 던지기
			break;
		default:
			assert(false);
		}
	}

	void BealzebubFSM::DrawWarning(bool bIsWidth, unsigned int y)
	{
		using namespace d2dFramework;

		GameObject* player = getPlayer();
		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);

		GridComponent* playergrid = player->GetComponent<GridComponent>();
		GridComponent* bossgrid = boss->GetComponent<GridComponent>();

		GameObject* Warning = ObjectManager::GetInstance()->CreateObject(mProjectileObjectId++, static_cast<float>(mBeatManager->GetIntervalTime()));
		AnimationRenderer* sprite = Warning->CreateComponent<AnimationRenderer>(mProjectileObjectId++);
		GridComponent* grid = Warning->CreateComponent<GridComponent>(mProjectileObjectId++);
		Transform* transform = Warning->CreateComponent<Transform>(mProjectileObjectId++);

		Warning->SetObjectType(eObjectType::Item);
		sprite->SetTransformLayer(eTransformLayer::Grid);
		sprite->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(WARNING_ANI_KEY));
		sprite->SetSize({ 3000, 700 });

		grid->SetMaxX(bossgrid->GetMaxX());
		grid->SetMaxY(bossgrid->GetMaxY());
		grid->SetCellDistance(bossgrid->GetCellDistance());

		if (bIsWidth)
		{
			grid->SetPosition({ playergrid->GetPosition().X, y });
		}
		else
		{
			grid->SetPosition({ playergrid->GetPosition().X, playergrid->GetMaxY() / 2 });
			transform->SetRotate(90.f);
		}
	}

	ePhase BealzebubFSM::handlePhaseState()
	{
		switch (mCurrentPhase)
		{
		case ePhase::None:
			return ePhase::One;
			break;
		case ePhase::One:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Two;
			}
			break;
		case ePhase::Two:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Three;
			}
			break;
		case ePhase::Three:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Ending;
			}
			break;
		case ePhase::Four:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Ending;
			}
			break;
		case ePhase::Ending:
			// ending이 종료되면
			break;
		default:
			assert(false);
			break;
		}

		return mCurrentPhase;
	}

	void BealzebubFSM::enterPhase()
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
			ObjectHelper::CreateMonster({ 3, 3 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 4 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 8, 3 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 10, 4 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 7, 5 }, eMonsterType::Hold);

			break;
		}
		case ePhase::Two:
		{
			ObjectHelper::CreateMonster({ 2, 4 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 4, 5 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 4, 2 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 6, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 7, 4 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 9, 3 }, eMonsterType::Hold);

			break;
		}
		case ePhase::Three:
		{
			ObjectHelper::CreateMonster({ 2, 3 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 3, 5 }, eMonsterType::Hold);
			ObjectHelper::CreateMonster({ 5, 3 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 6, 5 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 8, 4 }, eMonsterType::Escape);
			ObjectHelper::CreateMonster({ 8, 2 }, eMonsterType::Attack);
			ObjectHelper::CreateMonster({ 10, 5 }, eMonsterType::Hold);

			break;
		}
		case ePhase::Four:
		{


			break;
		}
		case ePhase::Ending:
		{
			
			break;
		}
		default:
			assert(false);
			break;
		}
	}

	d2dFramework::GameObject* BealzebubFSM::getPlayer() const
	{
		using namespace d2dFramework;

		std::set<unsigned int> playerIds = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Player);
		assert(playerIds.size() == 1);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(*playerIds.begin());
		assert(player != nullptr);

		return player;
	}

	d2dFramework::GameObject* BealzebubFSM::createProjectile()
	{
		using namespace d2dFramework;

		mProjectileIds.insert(mProjectileObjectId);
		GameObject* projectile = ObjectManager::GetInstance()->CreateObject(mProjectileObjectId++);

		Transform* transform = projectile->CreateComponent<Transform>(mProjectileObjectId++);
		AnimationRenderer* renderer = projectile->CreateComponent<AnimationRenderer>(mProjectileObjectId++);
		GridComponent* grid = projectile->CreateComponent<GridComponent>(mProjectileObjectId++);
		GridMoveComponent* gridMove = projectile->CreateComponent<GridMoveComponent>(mProjectileObjectId++);
		CircleCollider* circleCollider = projectile->CreateComponent<CircleCollider>(mProjectileObjectId++);

		projectile->SetObjectType(eObjectType::Projectile);

		circleCollider->SetRadius(25);

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(boss != nullptr);
		GridComponent* bossGrid = boss->GetComponent<GridComponent>();

		renderer->SetSize({ 100, 100 });
		renderer->SetTransformLayer(eTransformLayer::Grid);
		renderer->SetIsActive(true);
		renderer->SetIsLoop(true);
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(FLY_ANI_KEY));

		grid->SetCellDistance(bossGrid->GetCellDistance());
		grid->SetMaxX(bossGrid->GetMaxX());
		grid->SetMaxY(bossGrid->GetMaxY());
		grid->SetPosition(bossGrid->GetPosition());
		gridMove->SetCurrentSpeed(static_cast<float>(mBeatManager->GetIntervalTime()));

		return projectile;
	}
}
