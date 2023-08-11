#include "SoundAsset.h"

#include <cassert>

namespace d2dFramework
{
	SoundAsset::SoundAsset(unsigned int id, FMOD::Sound* sound, bool bIsLoop)
		: BaseEntity(id)
		, mSound(sound)
		, mbIsLoop(false)
		, mChannel(nullptr)
		, mVolume(0.5f)
		, mFrequency(1.f)
		, mPitch(1.f)
		, mDefaultFrequency(0.f)
	{
		assert(sound != nullptr);
	}

	SoundAsset::~SoundAsset()
	{
	}

	void SoundAsset::Play(FMOD::System* system)
	{
		//mChannel->stop();
		system->playSound(mSound, nullptr, false, &mChannel);
		if (mDefaultFrequency == 0.f)
		{
			mChannel->getFrequency(&mDefaultFrequency);
		}

		SetVolume(mVolume);
		SetFrequency(mFrequency);
		SetPitch(mPitch);
	}

	void SoundAsset::Stop()
	{
		mChannel->stop();
	}
}