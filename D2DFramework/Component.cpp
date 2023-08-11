#include "Component.h"

#include "GameObject.h"

namespace d2dFramework
{
	Component::Component(unsigned int id, GameObject* owner)
		: BaseEntity(id)
		, mOwner(owner)
	{
	}

	void Component::SerializeOut(nlohmann::ordered_json& object)
	{
		object["OwnerObject_ID"] = mOwner->GetId();
		object["Component_ID"] = GetId();
	}

}
