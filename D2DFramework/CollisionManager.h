#pragma once

#include "BaseEntity.h"
#include "ICollideable.h"
#include "GameObject.h"
#include "Manifold.h"
#include "eObjectType.h"

#include <cassert>
#include <unordered_map>
#include <queue>
#include <set>

namespace d2dFramework
{
	class CollisionManager final : public BaseEntity
	{
	public:
		CollisionManager();
		~CollisionManager();
		CollisionManager(const CollisionManager&) = delete;
		CollisionManager& operator=(const CollisionManager&) = delete;

		void Init();
		void Update();
		void Release();

		inline void RegisterCollideable(ICollideable* collideable);
		inline void UnregisterCollideable(ICollideable* collideable);

		inline void SetIsCollision(eObjectType lhs, eObjectType rhs);

		inline bool GetIsCollision(eObjectType lhs, eObjectType rhs) const;

	private:
		enum { RESERVE_SIZE = 4096 };
		enum { ADJ_LIST_SIZE = 64 };

		std::unordered_map<unsigned int, std::unordered_set<unsigned int>> mOnCollisionAdjLists;
		std::queue<std::pair<unsigned int, unsigned int>> mOnEnterCollisionQueue;
		std::queue<std::pair<unsigned int, unsigned int>> mOnExitCollisionQueue;

		std::unordered_map<unsigned int, ICollideable*> mCollideableMaps;
		unsigned int mbIsCollisionFlags[static_cast<int>(eObjectType::Size)];
	};

	void CollisionManager::RegisterCollideable(ICollideable* collideable)
	{
		mCollideableMaps.insert({ collideable->GetId(), collideable });
		mOnCollisionAdjLists.insert({ collideable->GetId(), std::unordered_set<unsigned int>(ADJ_LIST_SIZE) });
	}

	void CollisionManager::UnregisterCollideable(ICollideable* collideable)
	{
		mCollideableMaps.erase(collideable->GetId());
		mOnCollisionAdjLists.erase(collideable->GetId());
	}

	void CollisionManager::SetIsCollision(eObjectType lhs, eObjectType rhs)
	{
		mbIsCollisionFlags[static_cast<unsigned int>(lhs)] |= (1 << static_cast<int>(rhs));
		mbIsCollisionFlags[static_cast<unsigned int>(rhs)] |= (1 << static_cast<int>(lhs));
	}

	bool CollisionManager::GetIsCollision(eObjectType lhs, eObjectType rhs) const
	{
		return (mbIsCollisionFlags[static_cast<unsigned int>(lhs)] >> static_cast<int>(rhs)) & 1;
	}
}