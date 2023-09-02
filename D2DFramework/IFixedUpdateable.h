#pragma once

#include <cassert>

namespace d2dFramework
{
	class ObjectManager;
	class GameObject;

	class IFixedUpdateable
	{
		friend class GameProcessor;

	public:
		IFixedUpdateable() = default;
		virtual ~IFixedUpdateable() = default;

		virtual void Init() = 0;
		virtual void fixedUpdate(float deltaTime) = 0;
		virtual void Release() = 0;

		virtual inline GameObject* GetGameObject() const = 0;
		virtual inline unsigned int GetId() const = 0;

	protected:
		inline ObjectManager* GetObjectManager() const;

	private:
		static void SetObjectManager(ObjectManager* objectManager);

	private:		
		static ObjectManager* mObjectManager;
	};

	ObjectManager* IFixedUpdateable::GetObjectManager() const
	{
		assert(mObjectManager != nullptr);
		return mObjectManager;
	}
}