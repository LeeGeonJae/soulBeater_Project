#pragma once

#include "GameProcessor.h"
#include "KeyInformation.h"

#include <queue>
#include <future>
#include <d2d1_1.h>

namespace d2dFramework
{
	class GameObject;
	class AsyncInputManager;
	class BeatManager;
	class GameObject;
	class EffectManager;
	class ParticleManager;
}

namespace soulBeater
{
	class PlayerFSM;
	class MonsterFSM;
	class BealzebubFSM;
	class YsBossFSM;
	class EveBossFSM;
	class LilithBossFSM;
	class UIManager;

	class SoulBeaterProcessor : public d2dFramework::GameProcessor
	{
	public:
		SoulBeaterProcessor(UINT width, UINT height, std::wstring name);
		virtual ~SoulBeaterProcessor();

		virtual void Init(HWND hwnd) override;
		virtual void Update();
		virtual void Release() override;

		static unsigned int GetTempId();

	private:
		void initScene();
		void initParticle();

		void updateFSM(float deltaTime, std::queue<d2dFramework::KeyInformation>& input);
		void updateEffect();               
		void handleSceneChange();

	private:
		enum { DEFAULT_BPM = 120 };

		static unsigned int mTempId;

		d2dFramework::BeatManager* mBeatManager;
		d2dFramework::EffectManager* mEffectManager;
		d2dFramework::ParticleManager* mParticleManager;
		UIManager* mUIManager;

		// Thread
		d2dFramework::AsyncInputManager* mAsyncInputManager;
		std::mutex mMutex;
		std::queue<double> mPlayerInputTime;
		std::future<void> mInputFuture;

		// FSM
		PlayerFSM* mPlayerFSM;
		MonsterFSM* mMonsterFSM;
		BealzebubFSM* mBealzebubFSM;
		YsBossFSM* mYsBossFSM;
		EveBossFSM* mEveBossFSM;
		LilithBossFSM* mLilithFSM;

		float mKnightDeathElapsed;
		float mGhostDeathElasped;
		float mSuccubusDeathElasped;
	};
}