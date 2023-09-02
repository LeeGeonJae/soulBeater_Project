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
	class EveBossComponent
		: public d2dFramework::Component
	{
		friend class YsBossFSM;

	public:
		EveBossComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~EveBossComponent() = default;

		virtual void Init() override;
		virtual void Release() override {};
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void AccumulateBeatElapsedTime(float deltaTime);

		inline void SetHp(int hp);
		inline void SetMoveableBeatCount(unsigned int speedInBeat);
		inline void SetBeatElapsedTime(float beatElapsedTime);
		inline void SetYsBossAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset);

		inline float GetBeatElapsedTime() const;
		inline unsigned int GetAccumulateCount(float interval) const;
		inline unsigned int GetMoveableBeatCount() const;
		inline int GetHp() const;
		inline d2dFramework::AnimationAsset* GetYsAnimation(std::wstring key);

	private:
		unsigned int	mMoveableBeatCount;
		int				mHp;
		float			mBeatElapsed;
		// attack type? 한칸, 혹은 범위, 전체 공격? 등등

		std::map<std::wstring, d2dFramework::AnimationAsset*> mBossYsAnimation;
	};

	using namespace d2dFramework;

	void EveBossComponent::SetHp(int hp)
	{
		mHp = hp;
	}
	inline void EveBossComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}
	void EveBossComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}
	void EveBossComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}
	void EveBossComponent::SetYsBossAnimationAsset(std::wstring animationName, d2dFramework::AnimationAsset* playerAnimationAsset)
	{
		mBossYsAnimation.insert(std::make_pair(animationName, playerAnimationAsset));
	}

	float EveBossComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}
	unsigned int EveBossComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}
	unsigned int EveBossComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}
	int EveBossComponent::GetHp() const
	{
		return mHp;
	}
	d2dFramework::AnimationAsset* EveBossComponent::GetYsAnimation(std::wstring key)
	{
		return mBossYsAnimation.find(key)->second;
	}
};