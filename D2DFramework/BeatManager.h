#pragma once

#include <Windows.h>

namespace d2dFramework
{
	class BeatManager
	{
	public:
		BeatManager(unsigned int bpm);
		~BeatManager() = default;
		BeatManager(const BeatManager& other) = default;
		BeatManager& operator=(const BeatManager& other) = default;

		inline void Init(); // ��Ʈ�� ����Ǹ� ȣ��Ǿ�� �Ѵ�.

		inline void SetBPM(unsigned int bpm);

		inline double GetBeatIntervalTime();
		inline LARGE_INTEGER GetStandardStartTime();

	private:
		unsigned int mBPM;
		double mBeatIntervalTime;
		LARGE_INTEGER mStandardStartTime;
	};

	void BeatManager::Init()
	{
		QueryPerformanceCounter(&mStandardStartTime);
		mBeatIntervalTime = 60.0 / mBPM;

	}

	void BeatManager::SetBPM(unsigned int bpm)
	{
		mBPM = bpm;
		Init();
	}

	double BeatManager::GetBeatIntervalTime()
	{
		return mBeatIntervalTime;
	}

	LARGE_INTEGER BeatManager::GetStandardStartTime()
	{
		return mStandardStartTime;
	}
}