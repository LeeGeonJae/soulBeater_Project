#pragma once

#include "BaseMonsterComponent.h"
#include "IUpdateable.h"
#include "GridPosition.h"

namespace d2dFramework
{
	class GmaeObject;
}

namespace soulBeater
{
	class FastFlyComponent
		: public d2dFramework::Component
		, public d2dFramework::IUpdateable
	{
	public:
		FastFlyComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~FastFlyComponent() = default;

		virtual void Init() override;
		virtual void Release() override;

		virtual void Update(float deltaTime) override;
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void SetWaringDuration(float warningDuration);
		inline void SetIsColumn(bool bIsColumn);
		inline void SetStartPosition(const d2dFramework::GridPosition& position);

		virtual inline d2dFramework::GameObject* GetGameObject() const override;
		virtual inline unsigned int GetId() const override;
		inline float GetWarningDuration() const;
		inline bool GetIsColumn() const;
		inline const d2dFramework::GridPosition& GetStartPosition() const;

	private:
		float mWarningDuration;
		bool mbIsColumn;
		d2dFramework::GridPosition mStartPosition;
	};

	void FastFlyComponent::SetWaringDuration(float warningDuration)
	{
		mWarningDuration = warningDuration;
	}
	void FastFlyComponent::SetIsColumn(bool bIsColumn)
	{
		mbIsColumn = bIsColumn;
	}
	void FastFlyComponent::SetStartPosition(const d2dFramework::GridPosition& position)
	{
		mStartPosition = position;
	}

	inline d2dFramework::GameObject* FastFlyComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	inline unsigned int FastFlyComponent::GetId() const
	{
		return BaseEntity::GetId();
	}
	float FastFlyComponent::GetWarningDuration() const
	{
		return mWarningDuration;
	}
	bool FastFlyComponent::GetIsColumn() const
	{
		return mbIsColumn;
	}
	const d2dFramework::GridPosition& FastFlyComponent::GetStartPosition() const
	{
		return mStartPosition;
	}
}