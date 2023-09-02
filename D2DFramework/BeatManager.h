#pragma once

#include "BaseEntity.h"
#include "MathHelper.h"

#include <Windows.h>

namespace d2dFramework
{
	class BeatManager final : public BaseEntity
	{
	public:
		BeatManager();
		~BeatManager() override = default;
		BeatManager(const BeatManager& other) = default;
		BeatManager& operator=(const BeatManager& other) = default;

		void Init(unsigned int bpm, LARGE_INTEGER frequency);

		inline void SetBPM(unsigned int bpm);

		inline double GetIntervalTime() const;
		inline LARGE_INTEGER GetLastBPMChaneTime() const;
		inline double GetIntervalDefferenceTime(LARGE_INTEGER currentTime) const;

	private:
		unsigned int mBPM;
		double mBeatIntervalTime;
		LARGE_INTEGER mLastBPMChangeTime;
		LARGE_INTEGER mFrequency;
	};

	void BeatManager::SetBPM(unsigned int bpm)
	{
		mBPM = bpm;
		QueryPerformanceCounter(&mLastBPMChangeTime);
		mBeatIntervalTime = 60.0 / mBPM;
	}

	double BeatManager::GetIntervalTime() const
	{
		return mBeatIntervalTime;
	}

	LARGE_INTEGER BeatManager::GetLastBPMChaneTime() const
	{
		return mLastBPMChangeTime;
	}

	double BeatManager::GetIntervalDefferenceTime(LARGE_INTEGER currentTime) const
	{
		double dfffTime = static_cast<double>(currentTime.QuadPart - mLastBPMChangeTime.QuadPart) / mFrequency.QuadPart;

		double remainderTime = MathHelper::GetDoubleRemainder(dfffTime, mBeatIntervalTime);
		double otherDefferenceTime = mBeatIntervalTime - remainderTime;

		return remainderTime < otherDefferenceTime ? remainderTime : otherDefferenceTime;
	}
}