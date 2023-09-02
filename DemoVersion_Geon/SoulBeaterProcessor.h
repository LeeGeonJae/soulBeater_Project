#pragma once

#include "GameProcessor.h"

#include "BeatManager.h"
#include "AsyncInputManager.h"

#include <future>

#include "PlayerFSM.h"

namespace d2dFramework
{
	class GameObject;
	class CircleCollider;
}

namespace TestGeon
{
	class PlayerFSM;
	class MonsterFSM;
	class YsBossFSM;
	class EveBossFSM;
	class LilithBossFSM;
	class BealzebubFSM;
}

namespace TestGeon
{
	const int BEAT_BPM = 100;

	class SoulBeaterProcessor : public d2dFramework::GameProcessor
	{
	public:
		SoulBeaterProcessor(UINT width, UINT height, std::wstring name);
		virtual ~SoulBeaterProcessor() = default;

		virtual void Init(HWND hwnd) override;
		void Update();
		virtual void Release() override;

	private:
		void initMonsterFSM();
		void initAsyncInputManager();

		void initPlayerSetting();
		void initPlayerAnimation();

		void initYsSetting();
		void initYsAnimation();

		void initEveSetting();
		void initEveAnimation();

		void initMonsterAnimation();

		void initLilithAnimation();
		void initLilithSetting();

		void initBealzebubAnimation();
		void initBealzebubSetting();

		void updateMonsterFSM();
		void updateColliderShow();
		void showAsyncInputState(std::queue<d2dFramework::KeyInformation>& testQueue);

		void tileLoadSetting();
		

	private:
		d2dFramework::GameObject* CreateMonster();

	public:
		static unsigned int GetTempId() { return mTempId++; }

	private:
		d2dFramework::BeatManager* mBeatManager;
		d2dFramework::AsyncInputManager* mAsyncInputManager;
		PlayerFSM* mplayerFSM;
		LilithBossFSM* mLilithBossFSM;
		YsBossFSM* mYsBossFSM;
		EveBossFSM * mEveBossFSM;
		BealzebubFSM* mBealzebubBossFSM;
		MonsterFSM * mMonsterFSM;
		std::future<void> mInputFuture;

		d2dFramework::CircleCollider* mMonstercollider;
		d2dFramework::GameObject* mPlayer;
		d2dFramework::GameObject* mYsBoss;
		d2dFramework::GameObject* mEveBoss;
		d2dFramework::GameObject* mLilithBoss;
		d2dFramework::GameObject* mBealzebubBoss;

	private:
		enum { DEFUALT_BPM = 100u };
		enum { GRID_DISTANCE = 110 };
		enum { MAX_X = 10 };
		enum { MAX_Y = 9 };

	public:
		static unsigned int mTempId;
	};
}