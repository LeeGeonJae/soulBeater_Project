#include "BeatManager.h"

#include "eFrameworkID.h"

namespace d2dFramework
{
	BeatManager::BeatManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::BeatManager))
		, mBPM(0u)
		, mBeatIntervalTime(0)
		, mLastBPMChangeTime()
		, mFrequency()
	{
	}

	void BeatManager::Init(unsigned int bpm, LARGE_INTEGER frequency)
	{
		SetBPM(bpm);
		mFrequency = frequency;
	}
}