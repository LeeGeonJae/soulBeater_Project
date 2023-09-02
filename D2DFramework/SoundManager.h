#pragma once

#include "BaseEntity.h"

#include <array>
#include <map>
#include <string>

#include <cassert>

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

	enum class eSoundType
	{
		BGM,
		Effect
	};

	enum class ePlayType
	{
		Overwrite,
		TryPlay
	};

	class SoundManager final : public BaseEntity
	{
		friend class GameProcessor;
	public:
		static SoundManager* GetInstance();

		SoundManager();
		~SoundManager();
		SoundManager(const SoundManager&) = delete;
		SoundManager& operator=(const SoundManager&) = delete;

		void Init();
		void release();

		SoundAsset* FindSoundAssetOrNull(unsigned int soundAssetId);
	
		void Play(unsigned int soundAssetID, eSoundType soundType, ePlayType playType, unsigned int index);
		void Pause(bool bIsPaused);
		void Pause(eSoundType soundType, unsigned int index, bool bIsPaused);
		void Stop();
		void Stop(eSoundType soundType, unsigned int index);

		void SetVolume(float volume);
		void SetBGMVolume(float volume);
		void SetEffectsVolume(float volume);
		void SetFrequency(float frequency);
		void SetPitch(float pitch);

		inline float GetVolume() const;
		inline float GetBgmVolume() const;
		inline float GetEffectVolume() const;

		inline float GetFrequencey() const;
		inline float GetPitch() const;

		inline SoundAsset* GetSoundAssetOrNull(unsigned int soundAssetId) const;

		SoundAsset* CreateSoundAsset(unsigned int soundAssetId, const std::string& filePath, bool bIsLoop);
	private:
		void DeleteSoundAsset(unsigned int soundAssetId);

		enum { CHANNEL_CAPACITY = 32 };
		enum { BGM_CHANNEL = 2 };
		enum { EFFECT_CHANNEL = 30 };
		static SoundManager* mInstance;

		float mFrequency;
		float mVolume;
		float mBgmVolume;
		float mEffectVolume;

		float mPitch;

		FMOD::System* mSystem;
		std::map<unsigned int, SoundAsset*> mSoundMaps;

		std::array<FMOD::Channel*, BGM_CHANNEL> mBGMs;
		std::array<FMOD::Channel*, EFFECT_CHANNEL> mEffects;

	


	};

	float SoundManager::GetVolume() const
	{
		return mVolume;
	}
	float SoundManager::GetBgmVolume() const
	{
		return mBgmVolume;
	}
	float SoundManager::GetEffectVolume() const
	{
		return mEffectVolume;
	}
	float SoundManager::GetFrequencey() const
	{
		return mFrequency;
	}
	float SoundManager::GetPitch() const
	{
		return mPitch;
	}
	SoundAsset* SoundManager::GetSoundAssetOrNull(unsigned int soundAssetId) const
	{
		auto iter = mSoundMaps.find(soundAssetId);

		if (iter == mSoundMaps.end())
		{
			return nullptr;
		}

		return iter->second;
	}
}