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
		TimeManager(const TimeManager&) = delete;
		TimeManager& operator=(const TimeManager&) = delete;

		void Init();
		void Update();

		inline float GetDeltaTime() const;
		inline unsigned int GetFPS() const;
		inline const LARGE_INTEGER& GetFrequency() const;
		inline const LARGE_INTEGER& GetCurTime() const;

	private:
		LARGE_INTEGER mCurTime;
		LARGE_INTEGER mPrevTime;
		LARGE_INTEGER mFrequency;
		unsigned int mFrameCount;
		unsigned int mFPS;

		float mDeltaTime;
	};

	float TimeManager::GetDeltaTime() const
	{
		return  mDeltaTime;
	}

	unsigned int TimeManager::GetFPS() const
	{
		return mFPS;
	}

	const LARGE_INTEGER& TimeManager::GetFrequency() const
	{
		return mFrequency;
	}
	inline const LARGE_INTEGER& TimeManager::GetCurTime() const
	{
		return mCurTime;
	}
}