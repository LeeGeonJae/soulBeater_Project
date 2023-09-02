#pragma once

#include "BaseMonsterComponent.h"
#include "IUpdateable.h"
#include "GridVector.h"

namespace d2dFramework
{
	class GmaeObject;
}

namespace soulBeater
{
	class FlyComponent
		: public d2dFramework::Component
		, public d2dFramework::IUpdateable
	{
	public:
		FlyComponent(unsigned int id, d2dFramework::GameObject* owner);
		virtual ~FlyComponent() = default;

		virtual void Init() override;
		virtual void Release() override;

		virtual void Update(float deltaTime) override;
		virtual void SerializeIn(nlohmann::ordered_json& object) override {};

		inline void SetDirection(const d2dFramework::GridVector& direction);

		virtual inline d2dFramework::GameObject* GetGameObject() const override;
		virtual inline unsigned int GetId() const override;
		inline const d2dFramework::GridVector& GetDirection() const;

	private:
		d2dFramework::GridVector mDirection;
	};

	void FlyComponent::SetDirection(const d2dFramework::GridVector& direction)
	{
		mDirection = direction;
	}

	inline d2dFramework::GameObject* FlyComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	inline unsigned int FlyComponent::GetId() const
	{
		return BaseEntity::GetId();
	}
	const d2dFramework::GridVector& FlyComponent::GetDirection() const
	{
		return mDirection;
	}
}