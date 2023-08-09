#include "TimeManger.h"
#include "eFrameworkID.h"

namespace d2dFramework
{
	TimeManager::TimeManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::TimeManager))
	{
		Init();
	}

	void TimeManager::Init()
	{
		QueryPerformanceFrequency(&mFrequency);
		QueryPerformanceCounter(&mCurTime);
		mPrevTime = mCurTime;
		mDeltaTime = 0.f;
		mFrameCount = 0;
		mFPS = 0;
	}

	void TimeManager::Update()
	{
		static float sTime = 0.f;
		sTime += GetDeltaTime();

		if (sTime >= 1.f)
		{
			sTime -= 1.f;
			mFPS = mFrameCount;
			mFrameCount = 0;
		}

		mDeltaTime = static_cast<float>(mCurTime.QuadPart - mPrevTime.QuadPart) / mFrequency.QuadPart;
		mPrevTime = mCurTime;
		QueryPerformanceCounter(&mCurTime);
		++mFrameCount;
	}
}