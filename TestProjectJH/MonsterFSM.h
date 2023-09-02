#pragma once

#include "BaseEntity.h"
#include "GridPosition.h"

#include <set>
#include <map>
#include <queue>
#include <d2d1.h>

namespace d2dFramework
{
	class BeatManager;
	class GameObject;
	class RenderManager;
}

namespace testProjectJH
{
	enum class eMonsterState
	{
		Idle,
		Move,
		Attack,
		Death
	};

	class MonsterFSM : public d2dFramework::BaseEntity
	{
	public:
		MonsterFSM();
		~MonsterFSM();

		void Init(d2dFramework::BeatManager* beatmanager);
		void Update(float deltaTime);
		void DebugRender(d2dFramework::RenderManager* renderManager, const D2D1::Matrix3x2F& camaraTransform);
		void Release();

		inline const std::map<unsigned int, std::pair<d2dFramework::GameObject*, eMonsterState>>& GetMonsterMaps() const;

	private:
		eMonsterState handleState(d2dFramework::GameObject* monster, eMonsterState monsterState);
		void update(float deltaTime, d2dFramework::GameObject* monster, eMonsterState monsterState);
		void enter(d2dFramework::GameObject* monster, eMonsterState monsterState);
		void exit(d2dFramework::GameObject* monster, eMonsterState monsterState);

		void handleWallPositions();
		void handlePositon(d2dFramework::GameObject* monster);
		void AddMoveIntent(d2dFramework::GameObject* monster);
		void handleMoveIndent();

		// helper
		d2dFramework::GameObject* getPlayer() const;
		unsigned int GetAxisMaxDistance(d2dFramework::GameObject* lhs, d2dFramework::GameObject* rhs) const;

	private:
		d2dFramework::BeatManager* mBeatManager;

		std::map<unsigned int, std::pair<d2dFramework::GameObject*, eMonsterState>> mMonsterMaps;

		std::map<d2dFramework::GridPosition, unsigned int> mMonsterGridPosition; // 그 셀의 소유권을 가진 오브젝트 
		std::multimap<unsigned int, d2dFramework::GridPosition> mMoveIntents;

		std::queue<unsigned int> mDeleteQueue;
	};

	const std::map<unsigned int, std::pair<d2dFramework::GameObject*, eMonsterState>>& MonsterFSM::GetMonsterMaps() const
	{
		return mMonsterMaps;
	}
}