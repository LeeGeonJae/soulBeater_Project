#pragma once

#include "BaseMonsterComponent.h"
#include "IUpdateable.h"
#include "GridVector.h"
#include "GridPosition.h"

namespace d2dFramework
{
	class GmaeObject;
}

namespace testProjectJH
{
	class BoomFlyComponent
		: public d2dFramework::Component
		, public d2dFramework::IUpdateable
	{
	public:
		BoomFlyComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~BoomFlyComponent() = default;

		virtual void Init() override;
		virtual void Release() override;

		virtual void Update(float deltaTime) override;
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void SetDirection(const d2dFramework::GridVector& direction);
		inline void SetCellCountToExplosion(unsigned int cellCountToExplosion);
		inline void SetStartPosition(const d2dFramework::GridPosition& startPosition);
		inline void SetIsRepeat(bool bIsRepeat);
		inline void SetProcessedExplosionCount(unsigned int processedExplosionCount);

		virtual inline d2dFramework::GameObject* GetGameObject() const override;
		virtual inline unsigned int GetId() const override;
		inline const d2dFramework::GridVector& GetDirection() const;
		inline unsigned int GetCellCountToExplosion() const;
		inline const d2dFramework::GridPosition& GetStartPosition() const;
		inline bool GetIsRepeat() const;
		inline unsigned int GetProcessedExplosionCount() const;

	private:
		void createExplosion();

	private:
		d2dFramework::GridPosition mStartPosition;
		d2dFramework::GridVector mDirection;
		unsigned int mCellCountToExplosion;
		bool mbIsRepeat;
		unsigned int mProcessedExplosionCount;
	};

	void BoomFlyComponent::SetDirection(const d2dFramework::GridVector& direction)
	{
		mDirection = direction;
	}
	void BoomFlyComponent::SetCellCountToExplosion(unsigned int cellCountToExplosion)
	{
		mCellCountToExplosion = cellCountToExplosion;
	}
	void BoomFlyComponent::SetStartPosition(const d2dFramework::GridPosition& startPosition)
	{
		mStartPosition = startPosition;
	}
	void BoomFlyComponent::SetIsRepeat(bool bIsRepeat)
	{
		mbIsRepeat = bIsRepeat;
	}
	void BoomFlyComponent::SetProcessedExplosionCount(unsigned int processedExplosionCount)
	{
		mProcessedExplosionCount = processedExplosionCount;
	}

	inline d2dFramework::GameObject* BoomFlyComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	inline unsigned int BoomFlyComponent::GetId() const
	{
		return BaseEntity::GetId();
	}
	const d2dFramework::GridVector& BoomFlyComponent::GetDirection() const
	{
		return mDirection;
	}
	unsigned int BoomFlyComponent::GetCellCountToExplosion() const
	{
		return mCellCountToExplosion;
	}
	const d2dFramework::GridPosition& BoomFlyComponent::GetStartPosition() const
	{
		return mStartPosition;
	}
	bool BoomFlyComponent::GetIsRepeat() const
	{
		return mbIsRepeat;
	}
	unsigned int BoomFlyComponent::GetProcessedExplosionCount() const
	{
		return mProcessedExplosionCount;
	}
}