#include "IFixedUpdateable.h"

#include "ObjectManager.h"

namespace d2dFramework
{
	ObjectManager* IFixedUpdateable::mObjectManager = nullptr;

	void IFixedUpdateable::SetObjectManager(ObjectManager* objectManager)
	{
		mObjectManager = objectManager;
	}

	void IFixedUpdateable::Init()
	{
		mObjectManager->RegisterFixedUpdateable(this);
	}

	void IFixedUpdateable::Release()
	{
		mObjectManager->UnregisterFixedUpdateable(this);
	}
}