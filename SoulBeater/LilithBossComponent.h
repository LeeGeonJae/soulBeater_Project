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

	class LilithBossComponent : public d2dFramework::Component
	{
		friend class LilithBossFSM;

	public:
		LilithBossComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~LilithBossComponent() = default;

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

	inline void LilithBossComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}
	void LilithBossComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}
	void LilithBossComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}

	float LilithBossComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}
	unsigned int LilithBossComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}
	unsigned int LilithBossComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}
};
