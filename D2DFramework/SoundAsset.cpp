#include "SoundAsset.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#include <cassert>


namespace d2dFramework
{
	SoundAsset::SoundAsset(unsigned int id, FMOD::Sound* sound)
		: BaseEntity(id)
		, mSound(sound)
	{
		assert(sound != nullptr);
		mSound->getLength(&mSoundLength, FMOD_TIMEUNIT_MS);
		mSoundLength = mSoundLength / 100;
		mMin = mSoundLength / 60;
		mSecond = mSoundLength % 60;
	}

	SoundAsset::~SoundAsset()
	{
		// mSound->release(); 해당 부분 터짐
	}

	FMOD::Channel* SoundAsset::Play(FMOD::System* system)
	{
		unsigned int asd;
		auto a = mSound->getLength(&asd,FMOD_TIMEUNIT_MS);

		FMOD::Channel* channel;
		system->playSound(mSound, nullptr, false, &channel);
		return channel;
	}
}
