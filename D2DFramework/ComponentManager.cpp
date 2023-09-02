#include "ComponentManager.h"

#include "eFrameworkID.h"

namespace d2dFramework
{
	ComponentManager* ComponentManager::mInstance = nullptr;

	ComponentManager* ComponentManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	ComponentManager::ComponentManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::ComponentManager))
		, mComponentMaps(RESERVE_SIZE)
	{
	}

	ComponentManager::~ComponentManager()
	{
		release();
	}

	void ComponentManager::release()
	{
		mComponentMaps.clear();
	}
}
