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
	class BealzebubComponent : public d2dFramework::Component
	{
		friend class LilithBossFSM;

	public:
		BealzebubComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~BealzebubComponent() = default;

		virtual void Init() override;
		virtual void Release() override {};
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void AccumulateBeatElapsedTime(float deltaTime);

		inline void SetMoveableBeatCount(unsigned int speedInBeat);
		inline void SetBeatElapsedTime(float beatElapsedTime);

		inline float GetBeatElapsedTime() const;
		inline unsigned int GetAccumulateCount(float interval) const;
		inline unsigned int GetMoveableBeatCount() const;

	private:
		unsigned int	mMoveableBeatCount;
		float			mBeatElapsed;
	};

	using namespace d2dFramework;

	inline void BealzebubComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}
	void BealzebubComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}
	void BealzebubComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}

	float BealzebubComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}
	unsigned int BealzebubComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}
	unsigned int BealzebubComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}
};
