#pragma once

#include "Component.h"
#include "Collider.h"
#include "OBB.h"
#include "AABB.h"

namespace d2dFramework
{
	class GameObject;

	class OBBCollider final : public Component, public Collider
	{
	public:
		OBBCollider(unsigned int id, GameObject* owner);
		virtual ~OBBCollider() = default;

		void Init() override;
		void Release() override;

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		void UpdateCollider() override;
		bool CheckCollision(ICollideable* other, Manifold* outManifold) override;

		inline void SetSize(const Vector2& size);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline const OBB& GetWorldOBB() const;
		inline const Vector2& GetSize() const;

	private:
		Vector2 mSize;
		OBB mWorldOBB;
	};

	void OBBCollider::SetSize(const Vector2& size)
	{
		mSize = size;
	}

	GameObject* OBBCollider::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int OBBCollider::GetId() const
	{
		return BaseEntity::GetId();
	}
	const OBB& OBBCollider::GetWorldOBB() const
	{
		return mWorldOBB;
	}
	const Vector2& OBBCollider::GetSize() const
	{
		return mSize;
	}
}