#include "CollisionManager.h"

#include "eFrameworkID.h"
#include "EventManager.h"

#include <string>

namespace d2dFramework
{
	CollisionManager::CollisionManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::CollisionManager))
		, mOnCollisionAdjLists(RESERVE_SIZE)
		, mOnEnterCollisionQueue()
		, mOnExitCollisionQueue()
		, mCollideableMaps(RESERVE_SIZE)
		, mbIsCollisionFlags{ 0, }
	{
	}

	CollisionManager::~CollisionManager()
	{
		Release();
	}

	void CollisionManager::Init()
	{
		Release();
		mbIsCollisionFlags[static_cast<unsigned int>(eObjectType::None)] = 0xFFFFFFFF;
	}

	void CollisionManager::Release()
	{
		mOnCollisionAdjLists.clear();

		while (!mOnEnterCollisionQueue.empty())
		{
			mOnEnterCollisionQueue.pop();
		}

		while (!mOnExitCollisionQueue.empty())
		{
			mOnExitCollisionQueue.pop();
		}

		mCollideableMaps.clear();
	}

	void CollisionManager::Update()
	{
		for (auto pair : mCollideableMaps)
		{
			pair.second->UpdateCollider();
		}

		Manifold manifold;
		for (auto iterI = mCollideableMaps.begin(); iterI != mCollideableMaps.end(); ++iterI)
		{
			ICollideable* lhsCollideable = iterI->second;

			auto iterJ = iterI;
			++iterJ;

			for (; iterJ != mCollideableMaps.end(); ++iterJ)
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

				auto lhsIter = mOnCollisionAdjLists.find(lhsId);
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

			auto lhsIter = mOnCollisionAdjLists.find(lhsId);
			assert(lhsIter != mOnCollisionAdjLists.end());
			lhsIter->second.insert(rhsId);

			auto rhsIter = mOnCollisionAdjLists.find(rhsId);
			assert(rhsIter != mOnCollisionAdjLists.end());
			rhsIter->second.insert(lhsId);
		}
		while (!mOnExitCollisionQueue.empty())
		{
			auto pair = mOnExitCollisionQueue.front();
			mOnExitCollisionQueue.pop();

			unsigned int lhsId = pair.first;
			unsigned int rhsId = pair.second;

			auto findLhs = mOnCollisionAdjLists.find(lhsId);
			assert(findLhs != mOnCollisionAdjLists.end());
			findLhs->second.erase(rhsId);

			auto findRhs = mOnCollisionAdjLists.find(rhsId);
			assert(findLhs != mOnCollisionAdjLists.end());
			findRhs->second.erase(lhsId);
		}
	}

}