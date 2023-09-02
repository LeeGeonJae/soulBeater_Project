#include "MonsterFSM.h"

#include "BaseMonsterComponent.h"
#include "eSoulBeaterID.h"
#include "ConstantTable.h"
#include "MathHelper.h"
#include "BossState.h"
#include "ConstantTable.h"

#include "IncludeManager.h"
#include "IncludeComponent.h"

#include <cmath>
#include <string>
#include <cassert>
#include <set>

namespace soulBeater
{

	MonsterFSM::MonsterFSM()
		: d2dFramework::BaseEntity(static_cast<unsigned int>(eSoulBeaterID::MonsterFSM))
		, mBeatManager(nullptr)
		, mMonsterMaps()
	{
	}
	MonsterFSM::~MonsterFSM()
	{
	}

	void MonsterFSM::Init(d2dFramework::BeatManager* beatmanager)
	{
		using namespace d2dFramework;

		mBeatManager = beatmanager;

		auto registerMonsterCallback = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(id);

			if (monster != nullptr)
			{
				assert(monster->GetComponent<Transform>() != nullptr);
				assert(monster->GetComponent<BaseMonsterComponent>() != nullptr);
				assert(monster->GetComponent<GridComponent>() != nullptr);
				assert(monster->GetComponent<GridMoveComponent>() != nullptr);

				mMonsterMaps.insert({ id, eMonsterState::Idle });
			}
		};
		EventManager::GetInstance()->RegisterEventHandler("RegisterMonster", GetId(), registerMonsterCallback);

		auto unregisterMonsterCallback = [this](const std::string& data) -> void
		{
			const int id = std::stoi(data);

			mMonsterMaps.erase(id);
		};
		EventManager::GetInstance()->RegisterEventHandler("UnregisterMonster", GetId(), unregisterMonsterCallback);
	}

	void MonsterFSM::Release()
	{
		using namespace d2dFramework;

		mBeatManager = nullptr;
		EventManager::GetInstance()->UnregisterEventHandler("RegisterMonster", GetId());
		EventManager::GetInstance()->UnregisterEventHandler("UnregisterMonster", GetId());
	}

	void MonsterFSM::Update(float deltaTime)
	{
		using namespace d2dFramework;

		mMonsterGridPosition.clear();
		mMoveIntents.clear();

		handleWallPositions();

		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(static_cast<unsigned int>(eSoulBeaterID::PlayerObject));

		if (player == nullptr)
		{
			return;
		}

		for (auto iter = mMonsterMaps.begin(); iter != mMonsterMaps.end(); ++iter)
		{
			unsigned int id = iter->first;
			GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(id);

			if (monster == nullptr)
			{
				continue;
			}

			assert(monster->GetComponent<BaseMonsterComponent>() != nullptr);
			eMonsterState& monsterState = iter->second;
			update(deltaTime, monster, monsterState);
		}

		handleMoveIndent();

		for (auto iter = mMonsterMaps.begin(); iter != mMonsterMaps.end(); ++iter)
		{
			unsigned int id = iter->first;
			GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(id);

			if (monster == nullptr)
			{
				continue;
			}

			assert(monster->GetComponent<BaseMonsterComponent>() != nullptr);
			eMonsterState monsterState = iter->second;
			eMonsterState nextState = handleState(monster, monsterState);

			if (nextState != monsterState)
			{
				exit(monster, monsterState);
				iter->second = nextState;
				enter(monster, nextState);
			}
		}

		while (!mDeleteQueue.empty())
		{
			unsigned int id = mDeleteQueue.front();
			mDeleteQueue.pop();

			mMonsterMaps.erase(id);
			ObjectManager::GetInstance()->DeleteObject(id);
		}
	}

	void MonsterFSM::DebugRender(d2dFramework::RenderManager* renderManager, const D2D1::Matrix3x2F& camaraTransform)
	{
		using namespace d2dFramework;

		for (auto iter = mMonsterMaps.begin(); iter != mMonsterMaps.end(); ++iter)
		{
			unsigned int id = iter->first;
			GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(id);

			if (monster == nullptr)
			{
				continue;
			}
			eMonsterState& monsterState = iter->second;

			BaseMonsterComponent* monsterComponent = monster->GetComponent<BaseMonsterComponent>();
			assert(monsterComponent != nullptr);
			GridComponent* monsterGridComponent = monster->GetComponent<GridComponent>();
			assert(monsterGridComponent != nullptr);

			unsigned int GridVector = monsterGridComponent->GetCellDistance();
			GridPosition gridPosition = monsterGridComponent->GetPosition();

			Vector2 detectSize = { static_cast<float>(GridVector * 2 * monsterComponent->GetDetectionArea()),static_cast<float>(GridVector * 2 * monsterComponent->GetDetectionArea()) };
			Vector2 attackSize = { static_cast<float>(GridVector * 2 * monsterComponent->GetAttackArea()),static_cast<float>(GridVector * 2 * monsterComponent->GetAttackArea()) };

			float x = static_cast<float>(GridVector * monsterGridComponent->GetMaxX() * 0.5f);
			float y = static_cast<float>(GridVector * monsterGridComponent->GetMaxY() * 0.5f);

			renderManager->SetTransform(D2D1::Matrix3x2F::Translation(static_cast<float>(GridVector * gridPosition.X), static_cast<float>(GridVector * gridPosition.Y)) * D2D1::Matrix3x2F::Translation(-x, -y) * camaraTransform);

			//renderManager->SetTransform(renderManager->GetGridObjectTransform());
			auto prevColor = renderManager->SetColor({ 0,1,0,1 });
			renderManager->DrawRectangle({ 0,0 }, detectSize);
			renderManager->SetColor({ 1,0,0,1 });
			renderManager->DrawRectangle({ 0,0 }, attackSize);
			renderManager->SetTransform(D2D1::Matrix3x2F::Identity());
			renderManager->SetColor(prevColor);
		}
	}


	eMonsterState MonsterFSM::handleState(d2dFramework::GameObject* monster, eMonsterState monsterState)
	{
		using namespace d2dFramework;

		GameObject* player = getPlayer();
		unsigned int distance = GetAxisMaxDistance(player, monster);
		BaseMonsterComponent* monsterComponent = monster->GetComponent<BaseMonsterComponent>();
		assert(monsterComponent != nullptr);

		switch (monsterState)
		{
		case eMonsterState::Idle:
			if (monsterComponent->GetHp() <= 0)
			{
				return eMonsterState::Death;
			}
			if (distance <= monsterComponent->GetAttackArea())
			{
				return eMonsterState::Attack;
			}
			if (distance <= monsterComponent->GetDetectionArea())
			{
				return eMonsterState::Move;
			}
			break;
		case eMonsterState::Move:
		{
			if (monsterComponent->GetHp() <= 0)
			{
				return eMonsterState::Death;
			}
			if (distance <= monsterComponent->GetAttackArea())
			{
				return eMonsterState::Attack;
			}
			if (distance > monsterComponent->GetDetectionArea())
			{
				return eMonsterState::Idle;
			}
		}
		break;
		case eMonsterState::Attack:
			// 플레이어 상태가 idle이면 어택 이벤트 던지고 죽음
			return eMonsterState::Death;
			// 플레이어 상태가 idle이 아니면 공격 실패
			return eMonsterState::Idle;
			// 공격이 종료되면 사망 상태로 전이
			break;
		case eMonsterState::Death:
			break;
		defualt:
			assert(false);
			break;
		}

		return monsterState;
	}

	void MonsterFSM::update(float deltaTime, d2dFramework::GameObject* monster, eMonsterState monsterState)
	{
		// update animation
		using namespace d2dFramework;

		handlePositon(monster);

		switch (monsterState)
		{
		case eMonsterState::Idle:
			break;
		case eMonsterState::Move:
		{
			BaseMonsterComponent* baseMonsterComponent = monster->GetComponent<BaseMonsterComponent>();
			assert(baseMonsterComponent != nullptr);

			double intervalTime = mBeatManager->GetIntervalTime();

			baseMonsterComponent->AccumulateBeatElapsedTime(deltaTime);
			unsigned int accumulateBeatCount = baseMonsterComponent->GetAccumulateCount(static_cast<float>(intervalTime));

			if (accumulateBeatCount < baseMonsterComponent->GetMoveableBeatCount())
			{
				return;
			}

			GridMoveComponent* gridMoveComponent = monster->GetComponent<GridMoveComponent>();

			if (!gridMoveComponent->GetIsCanMove())
			{
				return;
			}

			AddMoveIntent(monster);

			break;
		}
		case eMonsterState::Attack:
			break;
		case eMonsterState::Death:
			// 사망 로직이 끝나면 objectmanager에 삭제등록, 현재 fsm의 참조 제거
		{
			AnimationRenderer* renderer = monster->GetComponent<AnimationRenderer>();
			if (renderer->GetIsAnimationEnd())
			{
				mDeleteQueue.push(monster->GetId());
			}
		}
		break;
	defualt:
		assert(false);
		}
	}

	void MonsterFSM::enter(d2dFramework::GameObject* monster, eMonsterState monsterState)
	{
		using namespace d2dFramework;

		//SpriteRenderer* spriteRenderer = monster->GetComponent<SpriteRenderer>();

		//// chnage animation
		//switch (monsterState)
		//{
		//case eMonsterState::Idle:
		//	spriteRenderer->SetBaseColor({ 0,1,0,0.5f });
		//	break;
		//case eMonsterState::Move:
		//	spriteRenderer->SetBaseColor({ 0,1,0,1 });
		//	break;
		//case eMonsterState::Attack:
		//	spriteRenderer->SetBaseColor({ 1,0,0,1 });
		//	break;
		//case eMonsterState::Death:
		//	spriteRenderer->SetBaseColor({ 1,1,1,1 });
		//	ObjectManager::GetInstance()->DeleteObject(monster->GetId());
		//	mDeleteQueue.push(monster->GetId());
		//	break;
		//defualt:
		//	assert(false);
		//}

		AnimationRenderer* renderer = monster->GetComponent<AnimationRenderer>();
		renderer->SetFrameindex(0);
		renderer->SetAnimationIndex(static_cast<unsigned int>(monsterState));

		// chnage animation
		switch (monsterState)
		{
		case eMonsterState::Idle:
			break;
		case eMonsterState::Move:
			break;
		case eMonsterState::Attack:
			break;
		case eMonsterState::Death:
		{
			BaseMonsterComponent* monsterComponent = monster->GetComponent<BaseMonsterComponent>();
			eMonsterType monsterType = monsterComponent->GetMonsterType();
			renderer->SetAnimationIndex(static_cast<unsigned int>(monsterState));
			switch (monsterType)
			{
			case eMonsterType::Hold:
				SoundManager::GetInstance()->Play(EFFECT_SUCCUBUSHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 8);
				break;
			case eMonsterType::Attack:
				SoundManager::GetInstance()->Play(EFFECT_GHOSTHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 6);
				break;
			case eMonsterType::Escape:
				SoundManager::GetInstance()->Play(EFFECT_KNIGHTHIT_KEY, eSoundType::Effect, ePlayType::Overwrite, 7);
				break;
			default:
				break;
			}

			break;
		}
	defualt:
		{
			assert(false);
			break;
		}
		}
	}
	void soulBeater::MonsterFSM::exit(d2dFramework::GameObject* monster, eMonsterState monsterState)
	{
		switch (monsterState)
		{
		case eMonsterState::Idle:
			break;
		case eMonsterState::Move:
			break;
		case eMonsterState::Attack:
			break;
		case eMonsterState::Death:
			break;
		defualt:
			assert(false);
			break;
		}
	}

	void soulBeater::MonsterFSM::handleWallPositions()
	{
		using namespace d2dFramework;

		const std::set<unsigned int>& tiles = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Wall);

		for (auto iter = tiles.begin(); iter != tiles.end(); ++iter)
		{
			unsigned int id = *iter;

			GameObject* gameObject = ObjectManager::GetInstance()->FindObjectOrNull(id);
			assert(gameObject != nullptr);

			if (gameObject->GetComponent<AABBCollider>() != nullptr)
			{
				GridComponent* girdComponent = gameObject->GetComponent<GridComponent>();
				assert(girdComponent != nullptr);

				mMonsterGridPosition.insert({ girdComponent->GetPosition(), gameObject->GetId() });
			}
		}
	}

	void soulBeater::MonsterFSM::handlePositon(d2dFramework::GameObject* monster)
	{
		using namespace d2dFramework;

		GridMoveComponent* gridMonveComponent = monster->GetComponent<GridMoveComponent>();
		assert(gridMonveComponent != nullptr);

		unsigned int id = monster->GetId();
		const GridPosition& nextPosition = gridMonveComponent->GetNextPosition();

		// 모든 그리드 오브젝트는 다음 이동 공간에 대한 소유권을 갖는다.
		mMonsterGridPosition.insert({ nextPosition, id });
	}

	void soulBeater::MonsterFSM::handleMoveIndent()
	{
		using namespace d2dFramework;

		for (auto iter = mMoveIntents.begin(); iter != mMoveIntents.end();)
		{
			unsigned int id = iter->first;
			GridPosition nextPosition = iter->second;

			GameObject* monster = ObjectManager::GetInstance()->FindObjectOrNull(id);
			GridMoveComponent* gridMovementCompoent = monster->GetComponent<GridMoveComponent>();

			auto position = mMonsterGridPosition.find(nextPosition);

			if (position == mMonsterGridPosition.end())
			{
				BaseMonsterComponent* baseMonsterComponent = monster->GetComponent<BaseMonsterComponent>();
				double intervalTime = mBeatManager->GetIntervalTime();

				unsigned int accumulateCount = baseMonsterComponent->GetAccumulateCount(static_cast<float>(intervalTime));
				baseMonsterComponent->AccumulateBeatElapsedTime(static_cast<float>(-intervalTime * accumulateCount));

				gridMovementCompoent->MoveNextPositon(nextPosition);

				mMonsterGridPosition.erase(gridMovementCompoent->GetCurrentPosition());
				mMonsterGridPosition.insert({ nextPosition, id });

				auto range = mMoveIntents.equal_range(id);
				iter = mMoveIntents.erase(range.first, range.second);
			}
			else
			{
				++iter;
			}
		}
	}

	d2dFramework::GameObject* soulBeater::MonsterFSM::getPlayer() const
	{
		using namespace d2dFramework;

		std::set<unsigned int> playerIds = ObjectManager::GetInstance()->GetObjectTypeIDs(eObjectType::Player);
		assert(playerIds.size() == 1);
		GameObject* player = ObjectManager::GetInstance()->FindObjectOrNull(*playerIds.begin());
		assert(player != nullptr);

		return player;
	}

	void soulBeater::MonsterFSM::AddMoveIntent(d2dFramework::GameObject* monster)
	{
		using namespace d2dFramework;

		GameObject* player = getPlayer();

		GridComponent* playerGridComponent = player->GetComponent<GridComponent>();
		GridComponent* monsterGridComponent = monster->GetComponent<GridComponent>();

		GridPosition monsterPosition = monsterGridComponent->GetPosition();
		GridPosition playerPosition = playerGridComponent->GetPosition();

		GridVector distance = MathHelper::SubtractGirdPosition(playerPosition, monsterPosition);

		unsigned int monsterId = monster->GetId();

		BaseMonsterComponent* baseMonsterComponent = monster->GetComponent<BaseMonsterComponent>();
		eMonsterType monstertype = baseMonsterComponent->GetMonsterType();

		switch (monstertype)
		{
		case soulBeater::eMonsterType::Hold:
			break;
		case soulBeater::eMonsterType::Attack:
			if (distance.X != 0)
			{
				GridPosition moveXNext = { monsterPosition.X + (distance.X < 0 ? -1 : 1), monsterPosition.Y };
				mMoveIntents.insert({ monsterId, moveXNext });
			}
			if (distance.Y != 0)
			{
				GridPosition moveYNext = { monsterPosition.X,  monsterPosition.Y + (distance.Y < 0 ? -1 : 1) };
				mMoveIntents.insert({ monsterId, moveYNext });
			}
			break;
		case soulBeater::eMonsterType::Escape:
		{
			int checkX[4] = { 0, 1, 0, -1 };
			int checkY[4] = { 1, 0, -1, 0 };

			unsigned int currentDistnace = MathHelper::GetManhattanDistance(monsterPosition, playerPosition);

			for (int i = 0; i < 4; ++i)
			{
				GridPosition nextPosition = { monsterPosition.X + checkX[i], monsterPosition.Y + checkY[i] };

				if (nextPosition.X >= monsterGridComponent->GetMaxX() || nextPosition.Y >= monsterGridComponent->GetMaxY())
				{
					continue;
				}

				unsigned int nextDistnace = MathHelper::GetManhattanDistance(nextPosition, playerPosition);
				if (currentDistnace < nextDistnace)
				{
					mMoveIntents.insert({ monsterId, nextPosition });
				}
			}
		}
		break;
		}
	}

	unsigned int soulBeater::MonsterFSM::GetAxisMaxDistance(d2dFramework::GameObject* lhs, d2dFramework::GameObject* rhs) const
	{
		using namespace d2dFramework;

		GridComponent* lhsGrid = lhs->GetComponent<GridComponent>();
		GridComponent* rhsGrid = rhs->GetComponent<GridComponent>();

		return MathHelper::GetAxisMaxDistance(lhsGrid->GetPosition(), rhsGrid->GetPosition());
	}
}