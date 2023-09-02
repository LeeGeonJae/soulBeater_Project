#pragma once

#include "BaseEntity.h"
#include "BossState.h"

#include <d2d1helper.h>
#include <vector>

namespace d2dFramework
{
	class BeatManager;
	class RenderManager;
	class GameObject;
}

namespace soulBeater
{
	class MonsterFSM;

	class EveBossFSM : public d2dFramework::BaseEntity
	{
	public:
		EveBossFSM();
		~EveBossFSM();

		void Init(d2dFramework::BeatManager* beatmanager, MonsterFSM* monsterFSM, unsigned int ysID, unsigned int eveID, unsigned int playerID);
		void Update(float deltaTime);
		void DebugRender(const D2D1::Matrix3x2F& camaraTransform);
		void Release();

	private:
		void update(float deltaTime);
		void handleState(float deltaTime); // 다음 상태변화만을 감지하고
		void enter(); // 여기서 상태변환 시 시작 트리거 되는 동작
		void exit(); // 여기는 상태 변환 종료 빠져

		void handlePhaseState();
		void enterPhase();

		void attackPatton();

	private:
		// 플레이어와 보스, 현재 상태, 다음 상태
		d2dFramework::BeatManager* mBeatManager;
		MonsterFSM* mMonsterFSM;
		unsigned int mYsID;
		unsigned int mEveID;
		unsigned int mPlayerID;

		unsigned int mStartBeat;

		eBossState mCurrentState;
		eBossState mNextState;
		ePhase mCurrentPhase;
		ePhase mNextPhase;

		// 추가 지연 박자 ( 난이도 조절 )
		int mDelayBeat;

		// 액션 턴과 공격 랜덤 패턴
		eBossTurn mBossTurn;
		bool mbIsAttackComplete;
		int mRandAttack;

		unsigned int mPrevPlayerY;

		const int TURN_OF_ACTION = 1;
	};
}