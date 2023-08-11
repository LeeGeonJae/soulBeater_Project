#pragma once

#include <map>
#include <string>

#include "BaseEntity.h"

// FMOD
#ifndef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
using namespace FMOD;
#endif

namespace FMOD
{
	class System;
}

namespace d2dFramework
{
	class SoundAsset;

	class SoundManager final : public BaseEntity
	{
	public:
		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;

		void Init();
		void Release();

		void Play(unsigned int soundAssetID);
		void Stop(unsigned int soundAssetID);

		void SetVolumeAll(float volume);
		void SetFrequencyAll(float frequency);
		void SetPitchAll(float pitch);

		inline float GetVolume() const;
		inline float GetFrequencey() const;
		inline float GetPitch() const;
		SoundAsset* GetSoundAssetOrNull(unsigned int soundAssetId);

		SoundAsset* CreateSoundAsset(unsigned int soundAssetId, const std::string& filePath, bool bIsLoop);

	private:
		float mFrequency = 1.f;
		float mVolume = 1.f;
		float mPitch = 1.f;

		FMOD::System* mSystem;
		std::map<unsigned int, SoundAsset*> mSoundMap;
	};

	float SoundManager::GetVolume() const
	{
		return mVolume;
	}
	float SoundManager::GetFrequencey() const
	{
		return mFrequency;
	}
	float SoundManager::GetPitch() const
	{
		return mPitch;
	}
}