#include "Rigidbody.h"

#include "GameObject.h"
#include "Transform.h"

#include <algorithm>

namespace d2dFramework
{
	// float Rigidbody::mGravityWeight = 0.f;

	Rigidbody::Rigidbody(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mVelocity(0.f, 0.f)
		, mAcceleartion(0.f, 0.f)
		, mMass(1.f)
		, mInvMass(1 / mMass)
		, mCOR(0.2f)
		, mGravityScale(1.f)
	{
	}

	void Rigidbody::Init()
	{
		IFixedUpdateable::Init();
	}

	void Rigidbody::fixedUpdate(float deltaTime)
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();

		//AddVelocity({ 0, -mGravityWeight * mGravityScale });
		transform->AddTranslate(GetVelocity() * deltaTime);
	}

	void Rigidbody::Release()
	{
		IFixedUpdateable::Release();
	}

	void Rigidbody::SerializeIn(nlohmann::ordered_json& object)
	{
		//Component::SerializeIn(object);
		mVelocity.SetXY(object["mVelocity"][0], object["mVelocity"][1]);
		mAcceleartion.SetXY(object["mAcceleartion"][0], object["mAcceleartion"][1]);
		mMass = object["mMass"];
		mInvMass = object["mInvMass"];
		mCOR = object["mCOR"].get<float>();
		mGravityScale = object["mGravityScale"];
	}

	void Rigidbody::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "Rigidbody";
		Component::SerializeOut(object);
		object["mVelocity"] = { mVelocity.GetX(), mVelocity.GetY() };
		object["mAcceleartion"] = { mAcceleartion.GetX(),mAcceleartion.GetY() };
		object["mMass"] = mMass;
		object["mInvMass"] = mInvMass;
		object["mCOR"] = mCOR;
		object["mGravityScale"] = mGravityScale;
	}
}
