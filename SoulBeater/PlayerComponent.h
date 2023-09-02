#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "AnimationAsset.h"

#include "PlayerFSM.h"

#include <string>

namespace d2dFramework
{
	class GameObject;
	class GridMoveComponent;
	class AnimationAsset;
}

enum class ePlayerState
{
	Idle = 0,
	Dash,
	Attack,
	BeAttacked,
	Death,
};

namespace soulBeater
{
	class PlayerFSM;

	class PlayerComponent : public d2dFramework::Component
	{
		friend class PlayerFSM;

	public:
		PlayerComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~PlayerComponent() = default;

		virtual void Init() override;
		virtual void Release() override {};
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

	public:
		inline void AccumulateBeatElapsedTime(float deltaTime);

		inline void SetMoveableBeatCount(unsigned int speedInBeat);
		inline void SetBeatElapsedTime(float beatElapsedTime);
		inline void SetPlayerHp(int hp);
		inline void DamagedPlayer(int damage);
		inline void SetPlayerNextState(ePlayerState nextState);
		inline void SetPlayerCurrentState(ePlayerState currentState);
		inline void AddHitEffectElapsed(float deltaTime); // 추가 : 홍지환, 마지막 레이어에 랜더링하기 위해 추가함, 추후 개선 예정

		inline float GetBeatElapsedTime() const;
		inline unsigned int GetAccumulateCount(float interval) const;
		inline unsigned int GetMoveableBeatCount() const;
		inline int GetPlayerHp();
		inline ePlayerState GetPlayerNextState();
		inline ePlayerState GetPlayerCurrentState();
		inline float GetHitEffectElased() const;

	private:
		unsigned int					mMoveableBeatCount;
		float							mBeatElapsed;
		int								mHp;

		ePlayerState					mPlayerCurrentState;
		ePlayerState					mPlayerNextState;

		float mHitEffectElpased;
	};

	using namespace d2dFramework;

	inline void PlayerComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}

	void PlayerComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}

	void PlayerComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}

	inline void PlayerComponent::SetPlayerHp(int hp)
	{
		mHp = hp;
	}

	inline void PlayerComponent::DamagedPlayer(int damage)
	{
		mHp -= damage;
	}

	inline void PlayerComponent::SetPlayerNextState(ePlayerState nextState)
	{
		mPlayerNextState = nextState;
	}

	inline void PlayerComponent::SetPlayerCurrentState(ePlayerState currentState)
	{
		mPlayerCurrentState = currentState;
	}

	inline void PlayerComponent::AddHitEffectElapsed(float deltaTime)
	{
		if (mHitEffectElpased >= 0.f)
		{
			mHitEffectElpased -= deltaTime;
		}
	}

	float PlayerComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}

	unsigned int PlayerComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}

	unsigned int PlayerComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}

	inline int PlayerComponent::GetPlayerHp()
	{
		return mHp;
	}

	inline ePlayerState PlayerComponent::GetPlayerNextState()
	{
		return mPlayerNextState;
	}

	inline ePlayerState PlayerComponent::GetPlayerCurrentState()
	{
		return mPlayerCurrentState;
	}

	inline float PlayerComponent::GetHitEffectElased() const
	{
		return mHitEffectElpased;
	}
};