#pragma once

#include "BaseEntity.h"
#include "MonsterFSM.h"

#include <set>
#include <d2d1.h>

namespace d2dFramework
{
	class BeatManager;
	class GameObject;
	class RenderManager;
}

namespace testProjectJH
{
	enum class ePhase
	{
		None,
		One,
		Two,
		Three,
		Four,
		Ending
	};

	enum class eBealzebubAttack
	{
		Fly,
		FastFly,
		BoomFly,
		ChainBommFly,
		Size
	};

	enum class eBossState
	{
		Idle,
		Move,
		Attack,
		Hit,
		Death
	};

	class BealzebubFSM
	{
	public:
		BealzebubFSM();
		~BealzebubFSM();

		void Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int bossId);
		void Release();

		void Update(float deltaTime);
		void DebugRender(const D2D1::Matrix3x2F& camaraTransform);

	private:
		eBossState handleState();

		// state FSM
		void update(float deltaTime);
		void updateMove();
		void enter();
		void enterAttack(); // handle create Object setting
		void exit();

		// phase FSM
		ePhase handlePhaseState();
		void enterPhase();

		d2dFramework::GameObject* getPlayer() const;
		d2dFramework::GameObject* createProjectile();
		d2dFramework::GameObject* createMonster();

	private:
		d2dFramework::BeatManager* mBeatManager;

		MonsterFSM* mMonsterFSM;

		unsigned int mBossId;
		eBossState mCurrentState;
		ePhase mCurrentPhase;
		eBealzebubAttack mCurrentBealzebubAttack;

		bool mbIsHit; // 피격되었는가를 판단하는 플래그
		bool mbIsHitable; // 피격될 수 있는 상태인가를 판단하는 플래그

		unsigned int mProjectileObjectId = 20000012;

		std::set<unsigned int> mProjectileIds;

		float mAttackDelayTime; 
	};
}