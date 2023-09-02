#pragma once

#include "BaseEntity.h"
#include "MonsterFSM.h"

#include "BossState.h"

#include <set>
#include <d2d1.h>

namespace d2dFramework
{
	class BeatManager;
	class GameObject;
	class RenderManager;
}

namespace soulBeater
{
	enum class eBealzebubAttack
	{
		Fly,
		FastFly,
		FastFlyAll,
		BoomFly,
		ChainBommFly,
		Size
	};

	class BealzebubFSM : public d2dFramework::BaseEntity
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
		void enterAttack(eBealzebubAttack attackType); // handle create Object setting
		void exit();

		void DrawWarning(bool bIsWidth, unsigned int y);

		// phase FSM
		ePhase handlePhaseState();
		void enterPhase();

		d2dFramework::GameObject* getPlayer() const;
		d2dFramework::GameObject* createProjectile();

	private:
		d2dFramework::BeatManager* mBeatManager;

		MonsterFSM* mMonsterFSM;

		unsigned int			mBossId;
		eBossState				mCurrentState;
		eBossState				mNextState;
		ePhase					mCurrentPhase;
		eBealzebubAttack		mCurrentBealzebubAttack;

		bool					mbIsHit; // 피격되었는가를 판단하는 플래그
		bool					mbIsHitable; // 피격될 수 있는 상태인가를 판단하는 플래그

		unsigned int			mProjectileObjectId = 20000012;

		std::set<unsigned int>	mProjectileIds;

		float					mAttackDelayTime;

		unsigned int			mBeatCount;
	};
}