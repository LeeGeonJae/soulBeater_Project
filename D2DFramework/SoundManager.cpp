#include "SoundManager.h"

#include "eFrameworkID.h"
#include "SoundAsset.h"
#include "MathHelper.h"

namespace d2dFramework
{
	SoundManager* SoundManager::mInstance = nullptr;
	SoundManager* SoundManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	SoundManager::SoundManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::SoundManager))
		, mFrequency(1.f)
		, mVolume(1.f)
		, mBgmVolume(1.f)
		, mEffectVolume(.6f)
		, mPitch(1.f)
		, mSystem(nullptr)
		, mSoundMaps()
		, mBGMs{ nullptr, }
		, mEffects{ nullptr, }
	{
	}

	SoundManager::~SoundManager()
	{
		release();
	}

	void SoundManager::Init()
	{
		FMOD::System_Create(&mSystem);
		mSystem->init(CHANNEL_CAPACITY, FMOD_INIT_NORMAL, nullptr);
	}

	void SoundManager::release()
	{
		mSystem->close();
		mSystem->release();

		for (auto pair : mSoundMaps)
		{
			delete pair.second;
		}

		mSoundMaps.clear();
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

		SoundAsset* soundAsset = new SoundAsset(soundAssetId, sound);
		mSoundMaps.insert({ soundAssetId, soundAsset });

		return soundAsset;
	}

	SoundAsset* SoundManager::FindSoundAssetOrNull(unsigned int soundAssetId)
	{
		auto iter = mSoundMaps.find(soundAssetId);

		if (iter == mSoundMaps.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void SoundManager::DeleteSoundAsset(unsigned int soundAssetId)
	{
		mSoundMaps.erase(soundAssetId);
	}

	void SoundManager::Play(unsigned int soundAssetID, eSoundType soundType, ePlayType playType, unsigned int index)
	{
		auto find = mSoundMaps.find(soundAssetID);

		if (find == mSoundMaps.end())
		{
			return;
		}

		SoundAsset* soundAsset = find->second;
		float prevFrequency = 0.f;

		if (soundType == eSoundType::BGM)
		{
			if (playType == ePlayType::Overwrite)
			{
				if (mBGMs[index] != nullptr)
				{
					mBGMs[index]->stop();
				}

				mBGMs[index] = soundAsset->Play(mSystem);
				mBGMs[index]->setVolume(mBgmVolume);
				mBGMs[index]->setPitch(mPitch);
				mBGMs[index]->getFrequency(&prevFrequency);
				mBGMs[index]->setFrequency(prevFrequency * mFrequency);
			}
			else if (playType == ePlayType::TryPlay)
			{
				if (mBGMs[index] == nullptr)
				{
					mBGMs[index] = soundAsset->Play(mSystem);
					mBGMs[index]->setVolume(mBgmVolume);
					mBGMs[index]->setPitch(mPitch);
					mBGMs[index]->getFrequency(&prevFrequency);
					mBGMs[index]->setFrequency(prevFrequency * mFrequency);
				}
			}
		}
		else if (soundType == eSoundType::Effect)
		{
			if (playType == ePlayType::Overwrite)
			{
				if (mEffects[index] != nullptr)
				{
					mEffects[index]->stop();
				}

				mEffects[index] = soundAsset->Play(mSystem);
				mEffects[index]->setVolume(mEffectVolume);
				mEffects[index]->setPitch(mPitch);
				mEffects[index]->getFrequency(&prevFrequency);
				mEffects[index]->setFrequency(prevFrequency * mFrequency);
			}
			else if (playType == ePlayType::TryPlay)
			{
				if (mEffects[index] == nullptr)
				{
					mEffects[index] = soundAsset->Play(mSystem);
					mEffects[index]->setVolume(mEffectVolume);
					mEffects[index]->setPitch(mPitch);
					mEffects[index]->getFrequency(&prevFrequency);
					mEffects[index]->setFrequency(prevFrequency * mFrequency);
				}
			}
		}
	}

	void SoundManager::Pause(bool bIsPaused)
	{
		for (FMOD::Channel* channel : mBGMs)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setPaused(bIsPaused);
		}

		for (FMOD::Channel* channel : mEffects)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setPaused(bIsPaused);
		}
	}

	void SoundManager::Pause(eSoundType soundType, unsigned int index, bool bIsPaused)
	{
		if (soundType == eSoundType::BGM && mBGMs[index] != nullptr)
		{
			mBGMs[index]->setPaused(bIsPaused);
		}
		else if (soundType == eSoundType::Effect && mEffects[index] != nullptr)
		{
			mEffects[index]->setPaused(bIsPaused);
		}
	}

	void SoundManager::Stop()
	{
		for (auto iter = mBGMs.begin(); iter != mBGMs.end(); ++iter)
		{
			FMOD::Channel* channel = *iter;

			if (channel == nullptr)
			{
				continue;
			}

			channel->stop();
			*iter = nullptr;
		}

		for (auto iter = mEffects.begin(); iter != mEffects.end(); ++iter)
		{
			FMOD::Channel* channel = *iter;

			if (channel == nullptr)
			{
				continue;
			}

			channel->stop();
			*iter = nullptr;
		}
	}

	void SoundManager::Stop(eSoundType soundType, unsigned int index)
	{
		if (soundType == eSoundType::BGM && mBGMs[index] != nullptr)
		{
			mBGMs[index]->stop();
		}
		else if (soundType == eSoundType::Effect && mEffects[index] != nullptr)
		{
			mEffects[index]->stop();
		}
	}

	void SoundManager::SetVolume(float volume)
	{
		mVolume = MathHelper::Clamp(volume, 0.f, volume);

		for (FMOD::Channel* channel : mBGMs)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setVolume(mVolume);
		}

		for (FMOD::Channel* channel : mEffects)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setVolume(mVolume);
		}
	}

	void SoundManager::SetBGMVolume(float volume)
	{
		mBgmVolume = volume;
		for (FMOD::Channel* channel : mBGMs)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setVolume(mBgmVolume);
		}
	}

	void SoundManager::SetEffectsVolume(float volume)
	{
		mEffectVolume = volume;
		for (FMOD::Channel* channel : mEffects)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setVolume(mEffectVolume);
		}
	}

	void SoundManager::SetFrequency(float frequency)
	{
		mFrequency = MathHelper::Clamp(frequency, 0.f, frequency);

		for (FMOD::Channel* channel : mBGMs)
		{
			if (channel == nullptr)
			{
				continue;
			}

			float prevFrequency;
			channel->getFrequency(&prevFrequency);
			channel->setFrequency(prevFrequency * mFrequency);
		}

		for (FMOD::Channel* channel : mEffects)
		{
			if (channel == nullptr)
			{
				continue;
			}

			float prevFrequency;
			channel->getFrequency(&prevFrequency);
			channel->setFrequency(prevFrequency * mFrequency);
		}
	}

	void SoundManager::SetPitch(float pitch)
	{
		mPitch = MathHelper::Clamp(pitch, 0.f, pitch);

		for (FMOD::Channel* channel : mBGMs)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setPitch(mPitch);
		}

		for (FMOD::Channel* channel : mEffects)
		{
			if (channel == nullptr)
			{
				continue;
			}

			channel->setPitch(mPitch);
		}
	}
}