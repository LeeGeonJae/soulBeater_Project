#pragma once

#include "BaseEntity.h"

#include <Windows.h>

namespace d2dFramework
{
	class TimeManager final : public BaseEntity
	{
	public:
		TimeManager();
		~TimeManager() = default;

		void Init();
		void Update();

		inline float GetDeltaTime();
		inline unsigned int GetFPS();

	private:
		LARGE_INTEGER mCurTime;
		LARGE_INTEGER mPrevTime;
		LARGE_INTEGER mFrequency;
		unsigned int mFrameCount;
		unsigned int mFPS;

		float mDeltaTime;
	};

	float TimeManager::GetDeltaTime()
	{
		return  mDeltaTime;
	}

	unsigned int TimeManager::GetFPS()
	{
		return mFPS;
	}
}