#include "AABB.h"
#include "AABBCollider.h"
#include "CircleCollider.h"
#include "OBB.h"
#include "OBBCollider.h"
#include "Collision.h"
#include "GameObject.h"
#include "Transform.h"
#include "Manifold.h"

#include <cassert>
#include <cmath>

namespace d2dFramework
{
	CircleCollider::CircleCollider(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, Collider(eColliderType::Circle)
		, mRadius(1.f)
	{
	}

	void CircleCollider::Init()
	{
		Collider::Init();
	}

	void CircleCollider::Release()
	{
		Collider::Release();
	}

	void CircleCollider::SerializeIn(nlohmann::ordered_json& object)
	{
		//Component::SerializeIn(object);

		Collider::SerializeIn(object);
		 mRadius= object["mRadius"];
		 mWorldCircle.Center.SetXY(object["mWorldCircle.Center"][0], object["mWorldCircle.Center"][1]);
		 mWorldCircle.Radius = object["mWorldCircle.Radius"];
	}

	void CircleCollider::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "CircleCollider";
		Component::SerializeOut(object);
		Collider::SerializeOut(object);

		object["mRadius"] = mRadius;
		object["mWorldCircle.Center"] = { mWorldCircle.Center.GetX(), mWorldCircle.Center.GetY() };
		object["mWorldCircle.Radius"] = mWorldCircle.Radius;
	}

	void CircleCollider::UpdateCollider()
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();

		mWorldCircle = Collision::MakeCircle(mOffset, mRadius, transform->GetSTTansform());
	}

	bool CircleCollider::CheckCollision(ICollideable* other, Manifold* outManifold)
	{
		switch (other->GetColliderType())
		{
		case eColliderType::AABB:
		{
			AABBCollider* concreteOther = static_cast<AABBCollider*>(other);
			return Collision::CheckCircleToAABB(GetWorldCircle(), concreteOther->GetWorldAABB(), outManifold);
		}
		break;
		case eColliderType::OBB:
		{
			OBBCollider* concreteOther = static_cast<OBBCollider*>(other);
			return Collision::CheckOBBToCircle(concreteOther->GetWorldOBB(), GetWorldCircle(), outManifold);
		}
		break;
		case eColliderType::Circle:
		{
			CircleCollider* concreteOther = static_cast<CircleCollider*>(other);
			return Collision::CheckCircleToCircle(GetWorldCircle(), concreteOther->GetWorldCircle(), outManifold);
		}
		break;
		default:
			assert(false);
			return false;
			break;
		}
	}
}