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
		for (auto pair : mSoundMap)
		{
			pair.second->SetVolume(volume);
		}
	}

	void SoundManager::SetFrequencyAll(float frequency)
	{
		for (auto pair : mSoundMap)
		{
			unsigned int id = pair.first;
			SoundAsset* soundAsset = pair.second;

			soundAsset->SetFrequency(frequency);
		}
	}

	SoundAsset* SoundManager::CreateSoundAsset(unsigned int soundAssetId, const std::string& filePath)
	{
		FMOD::Sound* sound = nullptr;
		FMOD::Channel* channel = nullptr;
		FMOD::DSP* dsp = nullptr;

		mSystem->createDSPByType(FMOD_DSP_TYPE_DELAY, &dsp);
		mSystem->createSound(filePath.c_str(), FMOD_LOOP_OFF, NULL, &sound);
		mSystem->playSound(sound, nullptr, false, &channel);
		channel->stop();
		sound->setMode(FMOD_LOOP_OFF);

		SoundAsset* soundAsset = new SoundAsset(soundAssetId, sound, channel, dsp);
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