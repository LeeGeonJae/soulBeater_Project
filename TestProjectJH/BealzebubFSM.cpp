#include "BealzebubFSM.h"

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
#include "TestProjectJH.h"

#include <set>
#include <map>
#include <cassert>

namespace testProjectJH
{
	BealzebubFSM::BealzebubFSM()
		: mBeatManager(nullptr)
		, mBossId(static_cast<unsigned int>(-1))
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

		mCurrentState = eBossState::Move;
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

		update(deltaTime);
		eBossState nextState = handleState();

		if (mCurrentState != nextState)
		{
			exit();
			mCurrentState = nextState;
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
				if (mCurrentPhase == ePhase::Four)
				{
					return eBossState::Death;
				}

				return eBossState::Hit;
			}
			if ((distance.X == 0 || distance.Y == 0) && mAttackDelayTime < 0.f)
			{
				return eBossState::Attack;
			}
			if (bossGridMoveComponent->GetIsCanMove())
			{
				return eBossState::Move;
			}

			break;
		case eBossState::Move:
		{
			// if Monster가 모두 죽으면 
			GridMoveComponent* bossGridMoveComponent = boss->GetComponent<GridMoveComponent>();
			assert(bossGridMoveComponent != nullptr);

			GameObject* player = getPlayer();
			GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
			GridComponent* bossGridComponent = boss->GetComponent<GridComponent>();

			GridVector distance = MathHelper::SubtractGirdPosition(playerGridComponent->GetPosition(), bossGridComponent->GetPosition());

			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				if (mCurrentPhase == ePhase::Four)
				{
					return eBossState::Death;
				}

				return eBossState::Hit;
			}

			if ((distance.X == 0 || distance.Y == 0) && mAttackDelayTime < 0.f)
			{
				return eBossState::Attack;
			}

			break;
		}
		case eBossState::Attack:
		{
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				if (mCurrentPhase == ePhase::Four)
				{
					return eBossState::Death;
				}

				return eBossState::Hit;
			}

			AnimationRenderer* renderer = boss->GetComponent<AnimationRenderer>();
			if (renderer->GetIsAnimationEnd())
			{
				return eBossState::Idle;
			}
		}
		break;
		case eBossState::Hit:
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
				ObjectManager::GetInstance()->DeleteObject(mBossId);
				// EventManager::GetInstance()->ExcuteBroadcastEvent("BossDead", "");
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

		switch (mCurrentState)
		{
		case eBossState::Idle:
			break;
		case eBossState::Move:
			updateMove();
			break;
		case eBossState::Attack:
			break;
		case eBossState::Hit:
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

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(boss != nullptr);
		AnimationRenderer* bossAniComponent = boss->GetComponent<AnimationRenderer>();;
		bossAniComponent->SetFrameindex(0);
		bossAniComponent->SetAnimationIndex(static_cast<unsigned int>(mCurrentState));

		switch (mCurrentState)
		{
		case eBossState::Idle:
			bossAniComponent->SetIsLoop(true);
			break;
		case eBossState::Move:
			bossAniComponent->SetIsLeft(true);
			break;
		case eBossState::Attack:
			enterAttack();
			bossAniComponent->SetIsLoop(false);
			mCurrentBealzebubAttack = static_cast<eBealzebubAttack>((static_cast<int>(mCurrentBealzebubAttack) + 1) % static_cast<int>(eBealzebubAttack::Size));
			// 공격 오브젝트 생성
			break;
		case eBossState::Hit:
			bossAniComponent->SetIsLoop(false);
			// 피격 효과 생성 이벤트 던지기
			break;
		case eBossState::Death:
			bossAniComponent->SetIsLoop(false);
			// 죽음 효과 생성 이벤트 던지기
			break;
		default:
			assert(false);
		}
	}

	void BealzebubFSM::enterAttack()
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

		switch (mCurrentBealzebubAttack)
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

			if (direction.X == 0)
			{
				gridCompoents[0]->SetPosition({ position.X - 1, position.Y });
				gridCompoents[2]->SetPosition({ position.X + 1, position.Y });

				if (direction.Y > 0)
				{
					direction.Y = 1;
					gridCompoents[1]->SetPosition({ position.X, position.Y + 1 });
				}
				else
				{
					direction.Y = -1;
					gridCompoents[1]->SetPosition({ position.X, position.Y - 1 });
				}
			}
			else
			{
				direction.X = direction.X > 0 ? 1 : -1;
				gridCompoents[0]->SetPosition({ position.X, position.Y - 1 });
				gridCompoents[2]->SetPosition({ position.X, position.Y + 1 });

				if (direction.X > 0)
				{
					direction.X = 1;
					gridCompoents[1]->SetPosition({ position.X + 1, position.Y });
				}
				else
				{
					direction.X = -1;
					gridCompoents[1]->SetPosition({ position.X - 1, position.Y });
				}
			}

			FlyComponent* flyComponent = projectiles[0]->CreateComponent<FlyComponent>(mProjectileObjectId++);
			flyComponent->SetDirection(direction);
			flyComponent = projectiles[1]->CreateComponent<FlyComponent>(mProjectileObjectId++);
			flyComponent->SetDirection(direction);
			flyComponent = projectiles[2]->CreateComponent<FlyComponent>(mProjectileObjectId++);
			flyComponent->SetDirection(direction);

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

			fastflyCompoent->SetWaringDuration(mBeatManager->GetIntervalTime());
			fastflyCompoent->SetStartPosition(bossGridComponent->GetPosition());

			if (direction.Y == 0)
			{
				projectileGridComponent->SetPosition({ projectileGridComponent->GetMaxX() - 1 , projectileGridComponent->GetPosition().Y });
				fastflyCompoent->SetIsColumn(true);
			}
			else
			{
				projectileGridComponent->SetPosition({ projectileGridComponent->GetPosition().X, 0 });
				fastflyCompoent->SetIsColumn(false);
			}
			projectile->Init();

			break;
		}
		case eBealzebubAttack::BoomFly:
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
					direction.Y = 1;
				}
				else
				{
					boomFlyComponent->SetCellCountToExplosion(-direction.Y);
					direction.Y = -1;
				}
			}
			else
			{
				if (direction.X > 0)
				{
					boomFlyComponent->SetCellCountToExplosion(direction.X);
					direction.X = 1;
				}
				else
				{
					boomFlyComponent->SetCellCountToExplosion(-direction.X);
					direction.X = -1;
				}
			}
			boomFlyComponent->SetDirection(direction);
			boomFlyComponent->SetIsRepeat(false);
			projectile->Init();

			break;
		}
		case eBealzebubAttack::ChainBommFly:
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
				}
				else
				{
					direction.Y = -1;
				}
			}
			else
			{
				if (direction.X > 0)
				{
					direction.X = 1;
				}
				else
				{
					direction.X = -1;
				}
			}
			boomFlyComponent->SetDirection(direction);
			boomFlyComponent->SetIsRepeat(true);
			projectile->Init();

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
		case eBossState::Move:
			break;
		case eBossState::Attack:
			break;
		case eBossState::Hit:
			// 피격 효과 종료 이벤트 던지기
			break;
		case eBossState::Death:
			// 죽음 효과 종료 이벤트 던지기
			break;
		default:
			assert(false);
		}
	}

	ePhase BealzebubFSM::handlePhaseState()
	{
		switch (mCurrentPhase)
		{
		case testProjectJH::ePhase::None:
			return ePhase::One;
			break;
		case testProjectJH::ePhase::One:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Two;
			}
			break;
		case testProjectJH::ePhase::Two:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Three;
			}
			break;
		case testProjectJH::ePhase::Three:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Four;
			}
			break;
		case testProjectJH::ePhase::Four:
			if (mMonsterFSM->GetMonsterMaps().size() == 0)
			{
				return ePhase::Ending;
			}
			break;
		case testProjectJH::ePhase::Ending:
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
		case testProjectJH::ePhase::None:
			break;
		case testProjectJH::ePhase::One:
		{
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 1,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 1,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 2,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 2,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			break;
		}
		case testProjectJH::ePhase::Two:
		{
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 3,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 3,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 4,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 4,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			break;
		}
		break;
		case testProjectJH::ePhase::Three:
		{
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 5,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 5,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 6,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 6,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			break;
		}
		case testProjectJH::ePhase::Four:
		{
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 7,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 7,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 8,1 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 8,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			break;
		}
		case testProjectJH::ePhase::Ending:
		{
			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 2,2 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 4,4 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 6,6 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

			monster = createMonster();
			grid = monster->GetComponent<GridComponent>();
			grid->SetPosition({ 8,8 });
			monster->GetComponent<BaseMonsterComponent>()->SetMonsterType(eMonsterType::Hold);

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

		renderer->SetSize({ 50, 50 });

		renderer->SetTransformLayer(eTransformLayer::Grid);
		renderer->SetIsLoop(true);
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"flyMove"));

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
		assert(boss != nullptr);
		GridComponent* bossGrid = boss->GetComponent<GridComponent>();

		grid->SetCellDistance(bossGrid->GetCellDistance());
		grid->SetMaxX(bossGrid->GetMaxX());
		grid->SetMaxY(bossGrid->GetMaxY());
		grid->SetPosition(bossGrid->GetPosition());
		gridMove->SetCurrentSpeed(mBeatManager->GetIntervalTime());

		return projectile;
	}

	d2dFramework::GameObject* BealzebubFSM::createMonster()
	{
		using namespace d2dFramework;

		GameObject* monster = nullptr;
		Transform* transform = nullptr;
		AnimationRenderer* renderer = nullptr;
		GridComponent* grid = nullptr;
		GridMoveComponent* gridmove = nullptr;
		BaseMonsterComponent* baseMonsterComponent = nullptr;
		CircleCollider* collider = nullptr;

		monster = ObjectManager::GetInstance()->CreateObject(TestProjectJH::GetTempId());
		monster->SetObjectType(eObjectType::Enemy);
		transform = monster->CreateComponent<Transform>(TestProjectJH::GetTempId());
		renderer = monster->CreateComponent<AnimationRenderer>(TestProjectJH::GetTempId());
		renderer->SetTransformLayer(eTransformLayer::Grid);
		grid = monster->CreateComponent<GridComponent>(TestProjectJH::GetTempId());
		gridmove = monster->CreateComponent<GridMoveComponent>(TestProjectJH::GetTempId());
		baseMonsterComponent = monster->CreateComponent<testProjectJH::BaseMonsterComponent>(TestProjectJH::GetTempId());
		collider = monster->CreateComponent<CircleCollider>(TestProjectJH::GetTempId());

		renderer->SetSize({ 50,	50 });
		renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(L"belzebub"));
		renderer->SetIsLoop(true);

		GameObject* boss = ObjectManager::GetInstance()->FindObjectOrNull(mBossId);
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
}