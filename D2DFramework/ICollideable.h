#pragma once

#include "eColliderType.h"

#include <cassert>
#include <functional>

namespace d2dFramework
{
	class hRectangle;
	class GameObject;
	class CollisionManager;
	struct Manifold;

	class ICollideable
	{
		friend class GameProcessor;

	public:
		ICollideable() = default;
		virtual ~ICollideable() = default;

		virtual void Init() = 0;
		virtual void Release() = 0;

		virtual void UpdateCollider() = 0;
		virtual bool CheckCollision(ICollideable* other, Manifold* outManifold) = 0;
		virtual void HandleRigidbody(ICollideable* other, const Manifold& manifold) = 0;

		virtual void OnEnterCollision(ICollideable* other) = 0;
		virtual void OnCollision(ICollideable* other) = 0;
		virtual void OnExitCollision(ICollideable* other) = 0;

		virtual inline void RegisterOnEnterCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) = 0;
		virtual inline void RegisterOnCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) = 0;
		virtual inline void RegisterOnExitCollisionCallBack(unsigned int id, std::function<void(unsigned int)> callback) = 0;

		virtual inline void UnregisterOnEnterCollisionCallBack(unsigned int id) = 0;
		virtual inline void UnregisterOnCollisionCallBack(unsigned int id) = 0;
		virtual inline void UnregisterOnExitCollisionCallBack(unsigned int id) = 0;

		virtual inline GameObject* GetGameObject() const = 0;
		virtual inline eColliderType GetColliderType() const = 0;
		virtual inline unsigned int GetId() const = 0;

	protected:
		inline CollisionManager* GetCollisionManager();

	private:
		static void SetCollisionManager(CollisionManager* collisionManager);

	private:
		static CollisionManager* mCollisionManager;
	};

	CollisionManager* ICollideable::GetCollisionManager()
	{
		assert(mCollisionManager != nullptr);
		return mCollisionManager;
	}
}