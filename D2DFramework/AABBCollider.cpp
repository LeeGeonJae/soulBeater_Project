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

	void AABBCollider::SerializeIn(nlohmann::ordered_json& object)
	{
		Collider::SerializeIn(object);
		mSize.SetXY(object["mSize"][0], object["mSize"][1]);
		mWorldAABB.BottomRight.SetXY(object["mWorldAABB"][0][0], object["mWorldAABB"][0][1]);
		mWorldAABB.TopLeft.SetXY(object["mWorldAABB"][1][0], object["mWorldAABB"][1][1]);
	}

	void AABBCollider::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "AABBCollider";
		Component::SerializeOut(object);
		Collider::SerializeOut(object);
		object["mSize"] = { mSize.GetX(),mSize.GetY() };
		object["mWorldAABB"] = { {mWorldAABB.BottomRight.GetX(),mWorldAABB.BottomRight.GetY()},{mWorldAABB.TopLeft.GetX(),mWorldAABB.TopLeft.GetY()} };
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