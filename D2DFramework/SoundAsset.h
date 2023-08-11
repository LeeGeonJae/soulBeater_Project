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
		SoundAsset(unsigned int id, FMOD::Sound* sound, bool bIsLoop);
		~SoundAsset();

		void Play(FMOD::System* system);
		void Stop();

		inline void SetLoop(bool bIsLoop);
		inline void SetVolume(float volume);
		inline void SetDelay(float delayTimeInSecond);
		inline void SetFrequency(float frequency);
		inline void SetPitch(float pitch);

		inline bool GetLoop() const;
		inline float GetVolume() const;
		inline float GetFrequency() const;
		inline float GetPitch() const;

	private:
		FMOD::Sound* mSound;
		FMOD::Channel* mChannel;
		FMOD::DSP* mDsp;
		float mDefaultFrequency;

		bool mbIsLoop;
		float mVolume;
		float mFrequency;
		float mPitch;
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
		mChannel->setFrequency(mDefaultFrequency * frequency);

		float speed = 0.f;
		mSound->getMusicSpeed(&speed);
		mSound->setMusicSpeed(2);
	}
	void SoundAsset::SetPitch(float pitch)
	{
		mPitch = pitch;
		mChannel->setPitch(pitch);
	}

	bool SoundAsset::GetLoop() const
	{
		return mbIsLoop;
	}
	float SoundAsset::GetVolume() const
	{
		return mVolume;
	}
	float SoundAsset::GetFrequency() const
	{
		return mFrequency;
	}
	float SoundAsset::GetPitch() const
	{
		return mPitch;
	}
}