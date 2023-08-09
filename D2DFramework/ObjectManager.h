#pragma once

#include "BaseEntity.h"
#include "GameObject.h"
#include "Transform.h"

#include <cassert>
#include <unordered_map>
#include <queue>

namespace d2dFramework
{
	class ObjectManager final : public BaseEntity
	{
		friend class GameProcessor;

	public:
		static ObjectManager* GetInstance();

		inline GameObject* CreateObject(unsigned int id);
		inline GameObject* FindObjectOrNull(unsigned int id);
		inline void DeletObject(unsigned int id);

	private:
		ObjectManager();
		~ObjectManager();
		ObjectManager(const ObjectManager&) = delete;
		ObjectManager& operator=(const ObjectManager&) = delete;

		void handleObjectLifeSpan();
		void release();

	private:
		enum { RESERVE_SIZE = 4096 };

		static ObjectManager* mInstance;

		std::unordered_map<unsigned int, GameObject*> mValidObjectMap;
		std::queue<GameObject*> mCreateObject;
		std::queue<GameObject*> mDeleteObject;

		// GameObject object pool
	};

	GameObject* ObjectManager::CreateObject(unsigned int id)
	{
		auto iter = mValidObjectMap.find(id);
		assert(iter == mValidObjectMap.end());

		GameObject* gameObject = new GameObject(id);
		mValidObjectMap.insert({ id, gameObject });
		mCreateObject.push(gameObject);

		return gameObject;
	}

	GameObject* ObjectManager::FindObjectOrNull(unsigned int id)
	{
		auto iter = mValidObjectMap.find(id);
		if (iter == mValidObjectMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void ObjectManager::DeletObject(unsigned int id)
	{
		auto iter = mValidObjectMap.find(id);

		if (iter == mValidObjectMap.end())
		{
			return;
		}

		GameObject* gameObject = iter->second;
		mDeleteObject.push(gameObject);
	}
}