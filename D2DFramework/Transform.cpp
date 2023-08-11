#include "Transform.h"

namespace d2dFramework
{
	Transform::Transform(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mScale(1.f, 1.f)
		, mRotateInDegree(0.f)
		, mTranslate(0.f, 0.f)
	{
	}
	void Transform::SerializeIn(nlohmann::ordered_json& object)
	{

		mScale.SetXY(object["mScale"][0], object["mScale"][1]);
		mRotateInDegree = object["mRotateInDegree"];
		mTranslate.SetXY(object["mTranslate"][0], object["mTranslate"][1]);

	}
	void Transform::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "Transform";
		Component::SerializeOut(object);
		object["mScale"] = { mScale.GetX(),mScale.GetY() };
		object["mRotateInDegree"]=mRotateInDegree  ;
		object["mTranslate"] = { mTranslate.GetX(),mTranslate.GetY() };
	}


}
