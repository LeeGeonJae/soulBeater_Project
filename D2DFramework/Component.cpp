#include "Component.h"

namespace d2dFramework
{
	Component::Component(unsigned int id, GameObject* owner)
		: BaseEntity(id)
		, mOwner(owner)
	{
	}
}