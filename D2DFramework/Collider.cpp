#include "Collider.h"

#include "GameObject.h"
#include "Transform.h"
#include "Manifold.h"
#include "Rigidbody.h"
#include "Collision.h"
#include "OBBCollider.h"
#include "CircleCollider.h"

namespace d2dFramework
{
	Collider::Collider(eColliderType colliderType)
		: mbIsTrigger(false)
		, mOffset(0.f, 0.f)
		, mColliderType(colliderType)
	{
	}

	void Collider::SerializeOut(nlohmann::ordered_json& object)
	{
		object["Collider_bIsTrigger"] = mbIsTrigger;
		object["Collider_Offset"] = { mOffset.GetX(),mOffset.GetY() };
		object["Collider_Size"] = { mSize.GetX(),mSize.GetY() };
	}

	void Collider::SerializeIn(nlohmann::ordered_json& object)
	{
		mbIsTrigger = object["Collider_bIsTrigger"];
		mOffset.SetXY(object["Collider_Offset"][0], object["Collider_Offset"][1]);
		mSize.SetXY(object["Collider_Size"][0], object["Collider_Size"][1]);
	}

	void Collider::Init()
	{
		ICollideable::Init();
	}

	void Collider::Release()
	{
		ICollideable::Release();

		mOnEnterCollisionCallBaak.clear();
		mOnCollisionCallBaak.clear();
		mOnExitCollisionCallBaak.clear();
	}

	void Collider::HandleRigidbody(ICollideable* other, const Manifold& manifold)
	{
		Rigidbody* rigidBody = GetGameObject()->GetComponent<Rigidbody>();
		Rigidbody* otherRigidBody = other->GetGameObject()->GetComponent<Rigidbody>();

		if (rigidBody == nullptr && otherRigidBody == nullptr)
		{
			return;
		}

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		Transform* otherTransform = other->GetGameObject()->GetComponent<Transform>();

		if (otherRigidBody == nullptr)
		{
			transform->AddTranslate(manifold.CollisionNormal * -manifold.Penetration);

			float scalar = Vector2::Dot(manifold.CollisionNormal, rigidBody->GetVelocity()) * (1 + rigidBody->GetCOR());

			rigidBody->AddVelocity(manifold.CollisionNormal * scalar);

			return;
		}
		if (rigidBody == nullptr)
		{
			otherTransform->AddTranslate(manifold.CollisionNormal * manifold.Penetration);

			float scalar = Vector2::Dot(manifold.CollisionNormal, otherRigidBody->GetVelocity()) * -(1 + otherRigidBody->GetCOR());

			otherRigidBody->AddVelocity(manifold.CollisionNormal * scalar);

			return;
		}

		Vector2 rv = otherRigidBody->GetVelocity() - rigidBody->GetVelocity();
		float scalar = Vector2::Dot(rv, manifold.CollisionNormal);

		if (scalar > 0)
		{
			return;
		}

		float COR = min(rigidBody->GetCOR(), otherRigidBody->GetCOR());

		float j = -(1 + COR) * scalar;
		j /= rigidBody->GetInvMass() + otherRigidBody->GetInvMass();

		float massSum = rigidBody->GetMass() + otherRigidBody->GetMass();

		if (massSum == 0.f)
		{
			return;
		}

		// 미는 연산 넣으면 떨림 발생
		//transform->AddTranslate(manifold.CollisionNormal * -manifold.Penetration * otherRigidBody->GetMass() / massSum);
		//otherTransform->AddTranslate(manifold.CollisionNormal * manifold.Penetration * rigidBody->GetMass() / massSum);

		Vector2 impulse = manifold.CollisionNormal * j;
		rigidBody->AddVelocity(impulse * -rigidBody->GetInvMass());
		otherRigidBody->AddVelocity(impulse * otherRigidBody->GetInvMass());

		const float percent = 0.2f;
		const float slop = 0.01f;
		Vector2 correction = manifold.CollisionNormal * (std::max<float>(manifold.Penetration - slop, 0.f) / (rigidBody->GetInvMass() + otherRigidBody->GetInvMass()) * percent);

		rigidBody->AddVelocity(correction * -rigidBody->GetInvMass());
		otherRigidBody->AddVelocity(correction * otherRigidBody->GetInvMass());

	}

	void Collider::OnEnterCollision(ICollideable* other)
	{
		for (auto iter : mOnEnterCollisionCallBaak)
		{
			iter.second(other->GetGameObject()->GetId());
		}

	}

	void Collider::OnCollision(ICollideable* other)
	{
		for (auto iter : mOnCollisionCallBaak)
		{
			iter.second(other->GetGameObject()->GetId());
		}
	}

	void Collider::OnExitCollision(ICollideable* other)
	{
		for (auto iter : mOnExitCollisionCallBaak)
		{
			iter.second(other->GetGameObject()->GetId());
		}
	}
}