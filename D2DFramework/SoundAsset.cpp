#include "SoundAsset.h"

#include <cassert>

namespace d2dFramework
{
	SoundAsset::SoundAsset(unsigned int id, FMOD::Sound* sound, FMOD::Channel* channel, FMOD::DSP* dsp)
		: BaseEntity(id)
		, mSound(sound)
		, mChannel(channel)
		, mDsp(dsp)
		, mbIsLoop(false)
		, mVolume(0.5f)
		, mFrequency(1.f)
	{
		assert(sound != nullptr);
		assert(channel != nullptr);
		assert(dsp != nullptr);
	}

	SoundAsset::~SoundAsset()
	{
	}

	void SoundAsset::Play(FMOD::System* system)
	{
		system->playSound(mSound, nullptr, false, &mChannel);
	}

	void SoundAsset::Stop()
	{
		mChannel->stop();
	}
}