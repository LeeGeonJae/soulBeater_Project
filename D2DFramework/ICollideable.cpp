#include "ICollideable.h"
#include "GameProcessor.h"
#include "CollisionManager.h"

namespace d2dFramework
{
	CollisionManager* ICollideable::mCollisionManager = nullptr;

	void ICollideable::SetCollisionManager(CollisionManager* collisionManager)
	{
		mCollisionManager = collisionManager;
	}

	void ICollideable::Init()
	{
		GetCollisionManager()->RegisterCollideable(this);
	}

	void ICollideable::Release()
	{
		GetCollisionManager()->UnregisterCollideable(this);
	}
}