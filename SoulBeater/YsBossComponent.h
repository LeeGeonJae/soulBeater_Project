#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "AnimationAsset.h"

#include <string>

namespace d2dFramework
{
	class GameObject;
	class GridMoveComponent;
	class AnimationAsset;
}

namespace soulBeater
{
	class YsBossComponent
		: public d2dFramework::Component
	{
		friend class YsBossFSM;

	public:
		YsBossComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~YsBossComponent() = default;

		virtual void Init() override;
		virtual void Release() override {};
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void AccumulateBeatElapsedTime(float deltaTime);

		inline void SetMoveableBeatCount(unsigned int speedInBeat);
		inline void SetBeatElapsedTime(float beatElapsedTime);
		inline void SetYsBossAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset);
		inline void SetBeatCount(unsigned int beatCount);
		inline void AddBeatCount(unsigned int number);

		inline float GetBeatElapsedTime() const;
		inline unsigned int GetAccumulateCount(float interval) const;
		inline unsigned int GetMoveableBeatCount() const;
		inline d2dFramework::AnimationAsset* GetYsAnimation(std::wstring key);
		inline unsigned int GetBeatCount();

	private:
		unsigned int	mMoveableBeatCount;
		float			mBeatElapsed;
		// attack type? 한칸, 혹은 범위, 전체 공격? 등등

		unsigned int	mYsEveBeatCount;

		std::map<std::wstring, d2dFramework::AnimationAsset*> mBossYsAnimation;
	};

	using namespace d2dFramework;

	inline void YsBossComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}
	void YsBossComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}
	void YsBossComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}
	void YsBossComponent::SetYsBossAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset)
	{
		mBossYsAnimation.insert(std::make_pair(animationName, playerAnimationAsset));
	}

	inline void YsBossComponent::SetBeatCount(unsigned int beatCount)
	{
		mYsEveBeatCount = beatCount;
	}

	inline void YsBossComponent::AddBeatCount(unsigned int number)
	{
		mYsEveBeatCount += number;
	}

	float YsBossComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}
	unsigned int YsBossComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}
	unsigned int YsBossComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}
	d2dFramework::AnimationAsset* YsBossComponent::GetYsAnimation(std::wstring key)
	{
		return mBossYsAnimation.find(key)->second;
	}
	inline unsigned int YsBossComponent::GetBeatCount()
	{
		return mYsEveBeatCount;
	}
};