#pragma once

#include "Component.h"
#include "Collider.h"
#include "AABB.h"

#include <unordered_map>


namespace d2dFramework
{
	class GameObject;

	class AABBCollider final : public Component, public Collider
	{
	public:
		AABBCollider(unsigned int id, GameObject* owner);
		~AABBCollider() override = default;

		void Init() override;
		void Release() override;

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;


		void UpdateCollider() override;
		bool CheckCollision(ICollideable* other, Manifold* outManifold) override;

		inline void SetSize(const Vector2& size);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline const Vector2& GetSize() const;
		inline const AABB& GetWorldAABB() const;

	private:
		Vector2 mSize;
		AABB mWorldAABB;
	};

	void AABBCollider::SetSize(const Vector2& size)
	{
		mSize = size;
	}

	GameObject* AABBCollider::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int AABBCollider::GetId() const
	{
		return BaseEntity::GetId();
	}
	const Vector2& AABBCollider::GetSize() const
	{
		return mSize;
	}
	const AABB& AABBCollider::GetWorldAABB() const
	{
		return mWorldAABB;
	}
}
