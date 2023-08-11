#pragma once

#include "Component.h"
#include "Collider.h"
#include "Circle.h"

namespace d2dFramework
{
	class GameObject;

	class CircleCollider final : public Component, public Collider
	{
	public:
		CircleCollider(unsigned int id, GameObject* owner);
		~CircleCollider() override = default;

		void Init() override;
		void Release() override;

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		void UpdateCollider() override;
		bool CheckCollision(ICollideable* other, Manifold* outManifold) override;

		inline void SetRadius(float radius);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline float GetRadius() const;
		inline const Circle& GetWorldCircle() const;

	private:
		float mRadius;
		Circle mWorldCircle;
	};

	void CircleCollider::SetRadius(float radius)
	{
		mRadius = radius;
	}

	GameObject* CircleCollider::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int CircleCollider::GetId() const
	{
		return BaseEntity::GetId();
	}
	float CircleCollider::GetRadius() const
	{
		return mRadius;
	}
	const Circle& CircleCollider::GetWorldCircle() const
	{
		return mWorldCircle;
	}
}