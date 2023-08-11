#include "SoundManager.h"

#include "eFrameworkID.h"
#include "SoundAsset.h"

namespace d2dFramework
{
	SoundManager::SoundManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::SoundManager))
		, mSystem(nullptr)
	{
	}

	SoundManager::~SoundManager()
	{
		Release();
	}

	void SoundManager::Init()
	{
		FMOD::System_Create(&mSystem);
		mSystem->init(32, FMOD_INIT_NORMAL, nullptr);
		mSystem->init(32, FMOD_INIT_NORMAL, nullptr); // 한 번에 실행될 수 있는 최대 채널 수를 의미한다.
	}

	void SoundManager::Release()
	{
		mSystem->close();
		mSystem->release();

		for (auto pair : mSoundMap)
		{
			delete pair.second;
		}

		mSoundMap.clear();
	}

	void SoundManager::Play(unsigned int soundAssetID)
	{
		auto find = mSoundMap.find(soundAssetID);

		if (find == mSoundMap.end())
		{
			return;
		}

		find->second->Play(mSystem);
		SoundAsset* soundAsset = find->second;

		soundAsset->Play(mSystem);
		soundAsset->SetFrequency(mFrequency);
		soundAsset->SetVolume(mVolume);
	}

	void SoundManager::Stop(unsigned int soundAssetID)
	{
		auto find = mSoundMap.find(soundAssetID);

		if (find == mSoundMap.end())
		{
			return;
		}

		find->second->Stop();
	}

	void SoundManager::SetVolumeAll(float volume)
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

		for (auto pair : mSoundMap)
		{
			pair.second->SetVolume(volume);
			pair.second->SetVolume(mVolume);
		}
	}

	void SoundManager::SetFrequencyAll(float frequency)
	{
		if (frequency < 0.f)
		{
			frequency = 0.f;
		}

		mFrequency = frequency;

		for (auto pair : mSoundMap)
		{
			unsigned int id = pair.first;
			SoundAsset* soundAsset = pair.second;

			soundAsset->SetFrequency(frequency);
			soundAsset->SetFrequency(mFrequency);
		}
	}
	void SoundManager::SetPitchAll(float pitch)
	{
		if (pitch < 0.f)
		{
			pitch = 0.f;
		}

		mPitch = pitch;

		for (auto pair : mSoundMap)
		{
			unsigned int id = pair.first;
			SoundAsset* soundAsset = pair.second;

			soundAsset->SetPitch(mPitch);
		}
	}

	SoundAsset* SoundManager::CreateSoundAsset(unsigned int soundAssetId, const std::string& filePath, bool bIsLoop)
	{
		FMOD::Sound* sound = nullptr;

		if (bIsLoop)
		{
			mSystem->createSound(filePath.c_str(), FMOD_LOOP_NORMAL, NULL, &sound);
		}
		{
			mSystem->createSound(filePath.c_str(), FMOD_LOOP_OFF, NULL, &sound);
		}

		SoundAsset* soundAsset = new SoundAsset(soundAssetId, sound, bIsLoop);
		mSoundMap.insert({ soundAssetId, soundAsset });

		return soundAsset;
	}

	SoundAsset* SoundManager::GetSoundAssetOrNull(unsigned int soundAssetId)
	{
		auto iter = mSoundMap.find(soundAssetId);

		if (iter == mSoundMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}
}