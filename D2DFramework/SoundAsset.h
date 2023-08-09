#pragma once

#include "BaseEntity.h"

#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

namespace FMOD
{
	class Sound;
	class Channel;
	class System;
}

namespace d2dFramework
{
	class SoundAsset : public BaseEntity
	{
	public:
		SoundAsset(unsigned int id, FMOD::Sound* sound, FMOD::Channel* channel, FMOD::DSP* dsp);
		~SoundAsset();

		void Play(FMOD::System* system);
		void Stop();

		inline void SetLoop(bool bIsLoop);
		inline void SetVolume(float volume);
		inline void SetDelay(float delayTimeInSecond);
		inline void SetFrequency(float frequency);

		inline bool GetLoop() const;
		inline float GetVolume() const;
		inline float GetFrequency(void) const;

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		FMOD::DSP* mDsp;

		bool mbIsLoop;
		float mVolume;
		float mFrequency;
	};

	void SoundAsset::SetLoop(bool bIsLoop)
	{
		mbIsLoop = bIsLoop;
	}
	void SoundAsset::SetVolume(float volume)
	{
		if (volume > 1.f)
		{
			volume = 1.f;
		}
		else if (volume < 0.f)
		{
			volume = 0.f;
		}

		mVolume = volume;
		mChannel->setVolume(mVolume);
	}
	void SoundAsset::SetDelay(float delayTimeInSecond)
	{
		if (delayTimeInSecond <= 0.f)
		{
			return;
		}

		mDsp->setParameterFloat(FMOD_DSP_DELAY_CH0, delayTimeInSecond * 1000.f);
		mChannel->addDSP(0, mDsp);
	}
	void SoundAsset::SetFrequency(float frequency)
	{
		if (frequency <= 0.f)
		{
			return;
		}

		mFrequency = frequency;
		mChannel->setPitch(frequency);
	}

	bool SoundAsset::GetLoop() const
	{
		return mbIsLoop;
	}
	float SoundAsset::GetVolume() const
	{
		return mVolume;
	}
	float SoundAsset::GetFrequency(void) const
	{
		return mFrequency;
	}
}