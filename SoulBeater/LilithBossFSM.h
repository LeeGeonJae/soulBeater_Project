#pragma once

#include "BaseEntity.h"
#include "BossState.h"

#include <d2d1helper.h>
#include <vector>
#include <set>

namespace d2dFramework
{
	class BeatManager;
	class RenderManager;
	class GameObject;
}

namespace soulBeater
{
	class MonsterFSM;

	class LilithBossFSM : public d2dFramework::BaseEntity
	{
	public:
		LilithBossFSM();
		~LilithBossFSM();

		void Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int bossId);
		void Update(float deltaTime);
		void Release();

	private:
		void update(float deltaTime);
		void handleState(float deltaTime); // 다음 상태변화만을 감지하고
		void enter(); // 여기서 상태변환 시 시작 트리거 되는 동작
		void exit(); // 여기는 상태 변환 종료 빠져

		void handlePhaseState();
		void enterPhase();

		void attackPatton(int pattonNumber);

		d2dFramework::GameObject* createProjectile();

		d2dFramework::GameObject* getPlayer();


	private:
		// 플레이어와 보스, 현재 상태, 다음 상태
		d2dFramework::BeatManager* mBeatManager;
		unsigned int mBossLilithId;

		eBossState mCurrentState;
		eBossState mNextState;
		ePhase mCurrentPhase;
		ePhase mNextPhase;

		MonsterFSM* mMonsterFSM;

		// 추가 지연 박자 ( 난이도 조절 )
		int mDelayBeat;
		unsigned int mBeatCount;

		// 액션 턴과 공격 랜덤 패턴
		eBossTurn mBossTurn;
		bool mbIsAttackComplete;

		unsigned int mProjectileObjectId = 20005012;

		std::set<unsigned int> mProjectileIds;

		const int TURN_OF_ACTION = 1;
	};
}