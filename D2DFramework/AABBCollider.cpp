#include "AABBCollider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Manifold.h"
#include "Rigidbody.h"
#include "Collision.h"
#include "OBBCollider.h"
#include "CircleCollider.h"

namespace d2dFramework
{
	AABBCollider::AABBCollider(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, Collider(eColliderType::AABB)
		, mSize(1.f, 1.f)
		, mWorldAABB{ Vector2(-0.5f, -0.5f), Vector2(0.5f, 0.5f) }
	{
	}

	void AABBCollider::Init()
	{
		Collider::Init();
	}

	void AABBCollider::Release()
	{
		Collider::Release();
	}

	void AABBCollider::UpdateCollider()
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();

		mWorldAABB = Collision::MakeAABB(mOffset, mSize, transform->GetSTTansform());
	}

	bool AABBCollider::CheckCollision(ICollideable* other, Manifold* outManifold)
	{
		switch (other->GetColliderType())
		{
		case eColliderType::AABB:
		{
			AABBCollider* concreteOther = static_cast<AABBCollider*>(other);
			return Collision::CheckAABBToAABB(GetWorldAABB(), concreteOther->GetWorldAABB(), outManifold);
		}
		break;
		case eColliderType::OBB:
		{
			OBBCollider* concreteOther = static_cast<OBBCollider*>(other);
			return Collision::CheckAABBToOBB(GetWorldAABB(), concreteOther->GetWorldOBB(), outManifold);
		}
		break;
		case eColliderType::Circle:
		{
			CircleCollider* concreteOther = static_cast<CircleCollider*>(other);
			return Collision::CheckAABBToCircle(GetWorldAABB(), concreteOther->GetWorldCircle(), outManifold);
		}
		break;
		default:
			assert(false);
			return false;
			break;
		}
	}
}