#include "IUpdateable.h"

#include "ObjectManager.h"

namespace d2dFramework
{
	ObjectManager* IUpdateable::mObjectManager = nullptr;

	void IUpdateable::SetObjectManager(ObjectManager* objectManager)
	{
		mObjectManager = objectManager;
	}

	void IUpdateable::Init()
	{
		mObjectManager->RegisterUpdateable(this);
	}

	void IUpdateable::Release()
	{
		mObjectManager->UnregisterUpdateable(this);
	}
}