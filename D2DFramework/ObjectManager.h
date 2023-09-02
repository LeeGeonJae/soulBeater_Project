#pragma once

#include "BaseEntity.h"
#include "GameObject.h"
#include "Transform.h"
#include "IFixedUpdateable.h"
#include "IUpdateable.h"
#include "eObjectType.h"

#include <cassert>
#include <set>
#include <map>
#include <unordered_map>
#include <queue>

namespace d2dFramework
{
	class ObjectManager final : public BaseEntity
	{
		friend class GameProcessor;
		friend class GameObject;
		friend class Scene;

	public:
		static ObjectManager* GetInstance();

		void Release();

		inline GameObject* CreateObject(unsigned int id, bool bIsIndepentent = false);
		inline GameObject* CreateObject(unsigned int id, float lifeSpan, bool bIsIndepentent = false);
		inline void DeleteObject(unsigned int id, bool bIsIndepentent = false);
		inline GameObject* FindObjectOrNull(unsigned int id, bool bIsIndepentent = false) const;
		inline void ClearObjects(bool bIsIndepentent = false);

		inline const std::set<unsigned int>& GetObjectTypeIDs(eObjectType objectType) const;

		inline void RegisterFixedUpdateable(IFixedUpdateable* fixedUpdateable);
		inline void RegisterUpdateable(IUpdateable* updateable);
		inline void UnregisterFixedUpdateable(IFixedUpdateable* fixedUpdateable);
		inline void UnregisterUpdateable(IUpdateable* updateable);

		inline const std::unordered_map<unsigned int, GameObject*>& GetIndependentObjectMap() const;
		inline const std::unordered_map<unsigned int, GameObject*>& GetObjectMaps() const;
		inline const std::queue<GameObject*>& GetCreateObjectQueue() const;
		inline const std::queue<GameObject*>& GetIndependentCreateObjectQueue() const;
		inline const std::queue<unsigned int>& GetDeleteObjectQueue() const;

	private:
		ObjectManager();
		~ObjectManager();
		ObjectManager(const ObjectManager&) = delete;
		ObjectManager& operator=(const ObjectManager&) = delete;

		void fixedUpdate(float deltaTime);
		void update(float deltaTime);

		void handleObjectLifeSpan();

	private:
		enum { SCENE_INDEPEDENT_OBJECT_RESER_SIZE = 512 };
		enum { RESERVE_SIZE = 512 };

		static ObjectManager* mInstance;

		std::unordered_map<unsigned int, GameObject*> mIndependentObjectMaps;
		std::unordered_map<unsigned int, GameObject*> mObjectMaps;
		std::queue<GameObject*> mCreateObjectQueue;
		std::queue<GameObject*> mIndepententCreateObjectQueue;
		std::queue<unsigned int> mDeleteObjectQueue;

		std::unordered_map<unsigned int, IFixedUpdateable*> mFixedUpdateable[GameObject::MAX_REFERENCE_DEPTH];
		std::unordered_map<unsigned int, IUpdateable*> mUpdateable[GameObject::MAX_REFERENCE_DEPTH];

		std::set<unsigned int> mObjectTypeIDs[static_cast<unsigned int>(eObjectType::Size)];

		std::map<unsigned int, float> mHasLifeSpanObjects;
	};

	const std::set<unsigned int>& ObjectManager::GetObjectTypeIDs(eObjectType objectType) const
	{
		assert(objectType != eObjectType::Size);
		return mObjectTypeIDs[static_cast<unsigned int>(objectType)];
	}

	GameObject* ObjectManager::FindObjectOrNull(unsigned int id, bool bIsIndepentent) const
	{
		if (!bIsIndepentent)
		{
			auto iter = mObjectMaps.find(id);
			if (iter == mObjectMaps.end())
			{
				return nullptr;
			}

			return iter->second;
		}
		else
		{
			auto iter = mIndependentObjectMaps.find(id);
			if (iter == mIndependentObjectMaps.end())
			{
				return nullptr;
			}

			return iter->second;
		}
	}

	GameObject* ObjectManager::CreateObject(unsigned int id, bool bIsIndepentent)
	{
		assert(mObjectMaps.find(id) == mObjectMaps.end());
		assert(mIndependentObjectMaps.find(id) == mIndependentObjectMaps.end());

		auto iter = mObjectMaps.find(id);
		GameObject* gameObject = new GameObject(id);

		if (!bIsIndepentent)
		{
			mCreateObjectQueue.push(gameObject);
		}
		else
		{
			mIndepententCreateObjectQueue.push(gameObject);
		}

		return gameObject;
	}

	inline GameObject* ObjectManager::CreateObject(unsigned int id, float lifeSpan, bool bIsIndepentent)
	{
		assert(mObjectMaps.find(id) == mObjectMaps.end());
		assert(mIndependentObjectMaps.find(id) == mIndependentObjectMaps.end());

		auto iter = mObjectMaps.find(id);
		GameObject* gameObject = new GameObject(id);

		if (!bIsIndepentent)
		{
			mCreateObjectQueue.push(gameObject);
		}
		else
		{
			mIndepententCreateObjectQueue.push(gameObject);
		}

		mHasLifeSpanObjects.insert({ id, lifeSpan });

		return gameObject;
	}

	void ObjectManager::DeleteObject(unsigned int id, bool bIsIndepentent)
	{
		mDeleteObjectQueue.push(id);
	}

	void ObjectManager::ClearObjects(bool bIsIndepentent)
	{
		if (!bIsIndepentent)
		{
			for (auto iter = mObjectMaps.begin(); iter != mObjectMaps.end(); ++iter)
			{
				GameObject* gameObject = iter->second;
				mDeleteObjectQueue.push(gameObject->GetId());
			}
		}
		else
		{
			for (auto iter = mIndependentObjectMaps.begin(); iter != mIndependentObjectMaps.end(); ++iter)
			{
				GameObject* gameObject = iter->second;
				mDeleteObjectQueue.push(gameObject->GetId());
			}
		}
	}

	const std::unordered_map<unsigned int, GameObject*>& ObjectManager::GetIndependentObjectMap() const
	{
		return mIndependentObjectMaps;
	}

	const std::unordered_map<unsigned int, GameObject*>& ObjectManager::GetObjectMaps() const
	{
		return mObjectMaps;
	}

	void ObjectManager::RegisterFixedUpdateable(IFixedUpdateable* fixedUpdateable)
	{
		GameObject* gameObject = fixedUpdateable->GetGameObject();
		mFixedUpdateable[gameObject->GetReferenceDepth()].insert({ fixedUpdateable->GetId(), fixedUpdateable });
	}
	void ObjectManager::RegisterUpdateable(IUpdateable* updateable)
	{
		GameObject* gameObject = updateable->GetGameObject();
		mUpdateable[gameObject->GetReferenceDepth()].insert({ updateable->GetId(), updateable });
	}

	void ObjectManager::UnregisterFixedUpdateable(IFixedUpdateable* fixedUpdateable)
	{
		GameObject* gameObject = fixedUpdateable->GetGameObject();
		mFixedUpdateable[gameObject->GetReferenceDepth()].erase(mFixedUpdateable->find(fixedUpdateable->GetId()));
	}
	void ObjectManager::UnregisterUpdateable(IUpdateable* updateable)
	{
		GameObject* gameObject = updateable->GetGameObject();
		mUpdateable[gameObject->GetReferenceDepth()].erase(mUpdateable->find(updateable->GetId()));
	}
	const std::queue<GameObject*>& ObjectManager::GetCreateObjectQueue() const
	{
		return mCreateObjectQueue;
	}
	const std::queue<GameObject*>& ObjectManager::GetIndependentCreateObjectQueue() const
	{
		return mIndepententCreateObjectQueue;
	}
	const std::queue<unsigned int>& ObjectManager::GetDeleteObjectQueue() const
	{
		return mDeleteObjectQueue;
	}
}