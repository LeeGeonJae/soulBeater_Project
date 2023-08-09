#include "CollisionManager.h"

#include "eFrameworkID.h"
#include "EventManager.h"

#include <string>

namespace d2dFramework
{
	CollisionManager::CollisionManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::CollisionManager))
		, mIsCollisionFlag{ 0, }
	{
	}

	CollisionManager::~CollisionManager()
	{
		Release();
	}

	void CollisionManager::Init()
	{
		mOnCollisionAdjList.reserve(RESERVE_SIZE);
		mCollideable.reserve(RESERVE_SIZE);
		mIsCollisionFlag[static_cast<unsigned int>(eObjectType::None)] = 0xFFFFFFFF;
	}

	void CollisionManager::Update()
	{
		for (auto pair : mCollideable)
		{
			pair.second->UpdateCollider();
		}

		Manifold manifold;
		for (auto iterI = mCollideable.begin(); iterI != mCollideable.end(); ++iterI)
		{
			ICollideable* lhsCollideable = iterI->second;

			auto iterJ = iterI;
			++iterJ;

			for (; iterJ != mCollideable.end(); ++iterJ)
			{
				ICollideable* rhsCollideable = iterJ->second;
				GameObject* lhsObject = lhsCollideable->GetGameObject();
				GameObject* rhsObject = rhsCollideable->GetGameObject();

				if (!GetIsCollision(lhsObject->GetObjectType(), rhsObject->GetObjectType()))
				{
					continue;
				}

				unsigned int lhsId = lhsCollideable->GetId();
				unsigned int rhsId = rhsCollideable->GetId();

				auto lhsIter = mOnCollisionAdjList.find(lhsId);
				bool bIsCollsionBefore = lhsIter->second.find(rhsId) != lhsIter->second.end();

				if (lhsCollideable->CheckCollision(rhsCollideable, &manifold))
				{
					lhsCollideable->HandleRigidbody(rhsCollideable, manifold);

					if (!bIsCollsionBefore)
					{
						mOnEnterCollisionQueue.push({ lhsId, rhsId });
						lhsCollideable->OnEnterCollision(rhsCollideable);
						rhsCollideable->OnEnterCollision(lhsCollideable);
					}
					else
					{
						lhsCollideable->OnCollision(rhsCollideable);
						rhsCollideable->OnCollision(lhsCollideable);
					}
				}
				else if (bIsCollsionBefore)
				{
					mOnExitCollisionQueue.push({ lhsId, rhsId });
					lhsCollideable->OnExitCollision(rhsCollideable);
					rhsCollideable->OnExitCollision(lhsCollideable);
				}
			}
		}

		while (!mOnEnterCollisionQueue.empty())
		{
			auto pair = mOnEnterCollisionQueue.front();
			mOnEnterCollisionQueue.pop();
			
			unsigned int lhsId = pair.first;
			unsigned int rhsId = pair.second;

			auto lhsIter = mOnCollisionAdjList.find(lhsId);
			assert(lhsIter != mOnCollisionAdjList.end());
			lhsIter->second.insert(rhsId);

			auto rhsIter = mOnCollisionAdjList.find(rhsId);
			assert(rhsIter != mOnCollisionAdjList.end());
			rhsIter->second.insert(lhsId);
		}
		while (!mOnExitCollisionQueue.empty())
		{
			auto pair = mOnExitCollisionQueue.front();
			mOnExitCollisionQueue.pop();

			unsigned int lhsId = pair.first;
			unsigned int rhsId = pair.second;

			auto findLhs = mOnCollisionAdjList.find(lhsId);
			assert(findLhs != mOnCollisionAdjList.end());
			findLhs->second.erase(rhsId);

			auto findRhs = mOnCollisionAdjList.find(rhsId);
			assert(findLhs != mOnCollisionAdjList.end());
			findRhs->second.erase(lhsId);
		}
	}

	void CollisionManager::Release()
	{
		mOnCollisionAdjList.clear();
	}
}