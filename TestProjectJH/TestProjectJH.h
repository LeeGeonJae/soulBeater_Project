#pragma once

#include "GameProcessor.h"
#include "KeyInformation.h"

#include <queue>
#include <future>
#include <d2d1_1.h>


namespace d2dFramework
{
	class AsyncInputManager;
	class BeatManager;
	class GameObject;
	class EffectManager;
	class ParticleManager;
	class PlayerFSM;
}

namespace testProjectJH
{
	class MonsterFSM;
	class BealzebubFSM;

	class TestProjectJH final : public d2dFramework::GameProcessor
	{
	public:
		TestProjectJH(UINT width, UINT height, std::wstring name);
		~TestProjectJH() override;

		void Init(HWND hwnd) override;
		void Update();
		void Release() override;

		static unsigned int GetTempId();
	
	private:
		void initAsyncInputManager();
		void initSound();
		void initEffect();
		void initParticle();
		void initMonsterFSM();
		void initPlayerFSM();
		void initBealzebubFSM();

		void updateAsyncInput();
		void updateSound();
		void updateEffect();
		void updateParticle();
		void updateMonsterFSM();

		d2dFramework::GameObject* createMonster();

		void showAsyncInputState(std::queue<d2dFramework::KeyInformation>& testQueue);

	private:
		enum { DEFUALT_BPM = 100u };
		enum { GRID_DISTANCE = 100 };
		enum { MAX_X = 10 };
		enum { MAX_Y = 10 };

		static unsigned int mTempId;

		d2dFramework::AsyncInputManager* mAsyncInputManager;
		d2dFramework::BeatManager* mBeatManager;
		d2dFramework::EffectManager* mEffectManager;
		std::mutex mMutex;
		std::queue<double> mPlayerInputTime;
		std::future<void> mInputFuture;

		ID2D1Bitmap* mBitmap;
		ID2D1Bitmap* mTempBitmap;

		unsigned int mSoundId;
		unsigned int mEffectId;

		//d2dFramework::Particle* mParticle;
		d2dFramework::ParticleManager* mParticleManager;
		unsigned int mParticleID[3];

		MonsterFSM* mMonsterFSM;

		d2dFramework::PlayerFSM* mplayerFSM;
		d2dFramework::GameObject* mPlayer;
		BealzebubFSM* mBealzebubFSM;
		unsigned int mBealzebubId = 203312;
	};
}