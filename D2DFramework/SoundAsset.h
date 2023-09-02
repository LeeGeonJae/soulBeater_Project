#pragma once

#include "BaseEntity.h"

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
		SoundAsset(unsigned int id, FMOD::Sound* sound);
		~SoundAsset();
		SoundAsset(const SoundAsset&) = delete;
		SoundAsset& operator=(const SoundAsset&) = delete;
		unsigned int GetLength() { return mSoundLength;}
		FMOD::Channel* Play(FMOD::System* system);

	private:
		FMOD::Sound* mSound;
		unsigned int mSoundLength;
		unsigned int mMin;
		unsigned int mSecond;
	};
}