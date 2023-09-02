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
		, mIndependentObjectMaps(SCENE_INDEPEDENT_OBJECT_RESER_SIZE)
		, mObjectMaps(RESERVE_SIZE)
		, mCreateObjectQueue()
		, mDeleteObjectQueue()
		, mFixedUpdateable{}
		, mUpdateable{}
		, mHasLifeSpanObjects()
	{
		for (unsigned int i = 0; i < static_cast<unsigned int>(GameObject::MAX_REFERENCE_DEPTH); ++i)
		{
			//mFixedUpdateable[i].reserve(RESERVE_SIZE);
			//mUpdateable[i].reserve(RESERVE_SIZE);
		}
	}

	ObjectManager::~ObjectManager()
	{
		Release();
	}

	void ObjectManager::Release()
	{
		while (!mCreateObjectQueue.empty())
		{
			GameObject* gameObject = mCreateObjectQueue.front();
			mCreateObjectQueue.pop();

			gameObject->Release();
			delete gameObject;
		}

		for (auto iter = mIndependentObjectMaps.begin(); iter != mIndependentObjectMaps.end();)
		{
			GameObject* gameObject = iter->second;
			iter = mIndependentObjectMaps.erase(iter);
			gameObject->Release();
			delete gameObject;
		}

		for (auto iter = mObjectMaps.begin(); iter != mObjectMaps.end();)
		{
			GameObject* gameObject = iter->second;
			iter = mObjectMaps.erase(iter);
			gameObject->Release();
			delete gameObject;
		}
	}

	void ObjectManager::fixedUpdate(float deltaTime)
	{
		for (int i = 0; i < GameObject::MAX_REFERENCE_DEPTH; ++i)
		{
			for (auto& pair : mFixedUpdateable[i])
			{
				pair.second->fixedUpdate(deltaTime);
			}
		}
	}

	void ObjectManager::update(float deltaTime)
	{
		for (int i = 0; i < GameObject::MAX_REFERENCE_DEPTH; ++i)
		{
			for (auto& pair : mUpdateable[i])
			{
				pair.second->Update(deltaTime);
			}
		}

		for (auto iter = mHasLifeSpanObjects.begin(); iter != mHasLifeSpanObjects.end();)
		{
			iter->second -= deltaTime;

			if (iter->second < 0.f)
			{
				DeleteObject(iter->first);
				DeleteObject(iter->first, true);
				iter = mHasLifeSpanObjects.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}

	void ObjectManager::handleObjectLifeSpan()
	{
		GameObject* gameObject = nullptr;

		while (!mCreateObjectQueue.empty())
		{
			gameObject = mCreateObjectQueue.front();
			mCreateObjectQueue.pop();

			gameObject->Init(false);
		}
		while (!mIndepententCreateObjectQueue.empty())
		{
			gameObject = mIndepententCreateObjectQueue.front();
			mIndepententCreateObjectQueue.pop();

			gameObject->Init(true);
		}

		while (!mDeleteObjectQueue.empty())
		{
			unsigned int id = mDeleteObjectQueue.front();
			mDeleteObjectQueue.pop();

			auto object = mObjectMaps.find(id);
			auto independentObject = mIndependentObjectMaps.find(id);

			if (object != mObjectMaps.end())
			{
				GameObject* gameObject = object->second;

				mObjectMaps.erase(object);
				gameObject->Release();
				delete gameObject;
			}
			else if (independentObject != mIndependentObjectMaps.end())
			{
				GameObject* gameObject = independentObject->second;

				mIndependentObjectMaps.erase(independentObject);
				gameObject->Release();
				delete gameObject;
			}

		}
	}
}