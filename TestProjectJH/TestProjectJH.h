#pragma once

#include "GameProcessor.h"

#include <queue>
#include <future>
#include <d2d1_1.h>

namespace d2dFramework
{
	class AsyncInputManager;
	class BeatManager;
	class GameObject;
	class EffectManager;
}

namespace testProjectJH
{
	class TestProjectJH : public d2dFramework::GameProcessor
	{
	public:
		TestProjectJH(UINT width, UINT height, std::wstring name);
		virtual ~TestProjectJH() = default;

		virtual void Init() override;
		virtual void Update();
		virtual void Release() override;

	private:
		void initAsyncInputManager();

		void updateAsyncInput();
		void updateSound();

	private:
		enum { DEFUALT_BPM = 100u };

		d2dFramework::AsyncInputManager* mAsyncInputManager;
		d2dFramework::BeatManager* mBeatManager;
		d2dFramework::EffectManager* mEffectManager;
		std::mutex mMutex;
		std::queue<double> mPlayerInputTime;
		std::future<void> mInputFuture;

		ID2D1Bitmap* mBitmap;

		unsigned int mSoundId;
		unsigned int mEffectId;
	};
}