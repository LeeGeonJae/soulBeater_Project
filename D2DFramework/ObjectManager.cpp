#include "ObjectManager.h"

#include "eFrameworkID.h"

namespace d2dFramework
{
	ObjectManager* ObjectManager::mInstance = nullptr;

	ObjectManager* ObjectManager::GetInstance()
	{
		assert(mInstance != nullptr);
		return mInstance;
	}

	ObjectManager::ObjectManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::ObjectManager))
	{
		mValidObjectMap.reserve(RESERVE_SIZE);
	}

	ObjectManager::~ObjectManager()
	{
		release();
	}

	void ObjectManager::handleObjectLifeSpan()
	{
		GameObject* gameObject = nullptr;

		while (!mCreateObject.empty())
		{
			gameObject = mCreateObject.front();
			mCreateObject.pop();

			gameObject->Init();
		}
		while (!mDeleteObject.empty())
		{
			gameObject = mDeleteObject.front();
			mDeleteObject.pop();

			gameObject->Release();
			delete gameObject;
		}
	}

	void ObjectManager::release()
	{
		for (auto iter = mValidObjectMap.begin(); iter != mValidObjectMap.end(); ++iter)
		{
			GameObject* gameObject = iter->second;

			delete gameObject;
		}

		mValidObjectMap.clear();
	}
}