#pragma once
#include "BaseEntity.h"
#include "Component.h"
#include <cassert>
#include <unordered_map>
#include <queue>

namespace d2dFramework
{
	// 단순히 탐색 용도로 활용한다. 수명 관리는 게임오브젝트 자체에서 함

	class ComponentManager final : public BaseEntity
	{
		friend class GameProcessor;
		friend class GameObject;

	public:
		static ComponentManager* GetInstance();

		inline Component* FindComponentOrNull(unsigned int id) const;

		inline const std::unordered_map<unsigned int, Component*>& GetComponentMaps() const;

	private:
		inline void registerComponent(Component* component);
		inline void unregisterComponent(unsigned int id);

		ComponentManager();
		~ComponentManager();
		ComponentManager(const ComponentManager&) = delete;
		ComponentManager& operator=(const ComponentManager&) = delete;

		void release();

	private:
		enum { RESERVE_SIZE = 4096 };
		static ComponentManager* mInstance;

		std::unordered_map<unsigned int, Component*> mComponentMaps;
	};

	Component* ComponentManager::FindComponentOrNull(unsigned int id) const
	{
		auto iter = mComponentMaps.find(id);
		if (iter == mComponentMaps.end())
		{
			return nullptr;
		}

		return iter->second;

	}

	void ComponentManager::registerComponent(Component* component)
	{
		auto iter = mComponentMaps.find(component->GetId());
		assert(iter == mComponentMaps.end());

		mComponentMaps.insert({ component->GetId(), component });
	}

	void ComponentManager::unregisterComponent(unsigned int id)
	{
		mComponentMaps.erase(id);
	}

	const std::unordered_map<unsigned int, Component*>& ComponentManager::GetComponentMaps() const
	{
		return mComponentMaps;
	}

}

