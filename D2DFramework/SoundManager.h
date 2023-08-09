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

		SoundAsset* CreateSoundAsset(unsigned int soundAssetId, const std::string& filePath);
		SoundAsset* GetSoundAssetOrNull(unsigned int soundAssetId);

	private:
		FMOD::System* mSystem;
		std::map<unsigned int, SoundAsset*> mSoundMap;
	};

}