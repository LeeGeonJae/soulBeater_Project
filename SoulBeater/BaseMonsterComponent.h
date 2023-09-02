#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "BossState.h"

namespace soulBeater
{

	class BaseMonsterComponent
		: public d2dFramework::Component
	{
		friend class MonsterFSM;

	public:
		BaseMonsterComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~BaseMonsterComponent() = default;

		virtual void Init() override;
		virtual void Release() override;
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void AccumulateBeatElapsedTime(float deltaTime);

		inline void SetBeatElapsedTime(float beatElapsedTime);
		inline void SetDetectionArea(unsigned int detectionArea);
		inline void SetAttackArea(unsigned int attackArea);
		inline void SetMoveableBeatCount(unsigned int speedInBeat);
		inline void SetHp(int hp);
		inline void SetMonsterType(eMonsterType monsterType);

		inline float GetBeatElapsedTime() const;
		inline unsigned int GetAccumulateCount(float interval) const;
		inline unsigned int GetDetectionArea() const;
		inline unsigned int GetAttackArea() const;
		inline unsigned int GetMoveableBeatCount() const;
		inline int GetHp() const;
		inline eMonsterType GetMonsterType() const;

	private:
		unsigned int mDectectionArea;
		unsigned int mAttackArea;
		unsigned int mMoveableBeatCount;
		int mHp;

		float mBeatElapsed;
		eMonsterType mMonsterType;
		// attack type? 한칸, 혹은 범위, 전체 공격? 등등
	};

	void BaseMonsterComponent::AccumulateBeatElapsedTime(float deltaTime)
	{
		mBeatElapsed += deltaTime;
	}

	void BaseMonsterComponent::SetBeatElapsedTime(float beatElapsedTime)
	{
		mBeatElapsed = beatElapsedTime;
	}
	void BaseMonsterComponent::SetDetectionArea(unsigned int detectionArea)
	{
		mDectectionArea = detectionArea;
	}
	void BaseMonsterComponent::SetAttackArea(unsigned int attackArea)
	{
		mAttackArea = attackArea;
	}
	void BaseMonsterComponent::SetMoveableBeatCount(unsigned int speedInBeat)
	{
		mMoveableBeatCount = speedInBeat;
	}
	void BaseMonsterComponent::SetHp(int hp)
	{
		mHp = hp;
	}
	void BaseMonsterComponent::SetMonsterType(eMonsterType monsterType)
	{
		mMonsterType = monsterType;
	}

	float BaseMonsterComponent::GetBeatElapsedTime() const
	{
		return mBeatElapsed;
	}
	unsigned int BaseMonsterComponent::GetAccumulateCount(float interval) const
	{
		assert(mBeatElapsed >= 0.f);
		return static_cast<unsigned int>(mBeatElapsed / interval);
	}
	unsigned int BaseMonsterComponent::GetDetectionArea() const
	{
		return mDectectionArea;
	}
	unsigned int BaseMonsterComponent::GetAttackArea() const
	{
		return mAttackArea;
	}
	unsigned int BaseMonsterComponent::GetMoveableBeatCount() const
	{
		return mMoveableBeatCount;
	}
	int BaseMonsterComponent::GetHp() const
	{
		return mHp;
	}
	eMonsterType BaseMonsterComponent::GetMonsterType() const
	{
		return mMonsterType;
	}
};