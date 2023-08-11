#include "OBBCollider.h"
#include "GameObject.h"

namespace d2dFramework
{
	OBBCollider::OBBCollider(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, Collider(eColliderType::OBB)
	{
	}

	void OBBCollider::Init()
	{
		ICollideable::Init();
	}

	void OBBCollider::UpdateCollider()
	{

	}

	bool OBBCollider::CheckCollision(ICollideable* other, Manifold* outManifold)
	{
		return true;
	}

	void OBBCollider::Release()
	{
		ICollideable::Release();
	}

	void OBBCollider::SerializeIn(nlohmann::ordered_json& object)
	{
		//Component::SerializeIn(object);

		Collider::SerializeIn(object);
		object["mSize"];
		mSize.SetXY(object["mSize"][0], object["mSize"][1]);
		for (int i=0; i<4;i++)
			mWorldOBB.mPoints[i] .SetXY(object["mWorldOBB"][i][0], object["mWorldOBB"][i][1]);




	}

	void OBBCollider::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "OBBCollider";
		Component::SerializeOut(object);
		Collider::SerializeOut(object);

		object["mSize"] = { mSize.GetX(),mSize.GetY() };
		object["mWorldOBB"] = {
			{mWorldOBB.mPoints[0].GetX(),mWorldOBB.mPoints[0].GetY()}
			,{mWorldOBB.mPoints[1].GetX(),mWorldOBB.mPoints[1].GetY()}
			,{mWorldOBB.mPoints[2].GetX(),mWorldOBB.mPoints[2].GetY()}
			,{mWorldOBB.mPoints[3].GetX(),mWorldOBB.mPoints[3].GetY()}
	};


	}
}
