#include "BeatManager.h"

namespace d2dFramework
{
	BeatManager::BeatManager(unsigned int bpm)
		: mBPM(bpm)
		, mBeatIntervalTime(0.f)
		, mStandardStartTime()
	{
		Init();
	}
}