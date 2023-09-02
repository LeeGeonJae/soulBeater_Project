#pragma once

#include <cassert>

namespace d2dFramework
{
	class ObjectManager;
	class GameObject;

	class IUpdateable
	{
		friend class GameProcessor;

	public:
		IUpdateable() = default;
		virtual ~IUpdateable() = default;

		virtual void Init() = 0;
		virtual void Update(float deltaTime) = 0;
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

	ObjectManager* IUpdateable::GetObjectManager() const
	{
		assert(mObjectManager != nullptr);
		return mObjectManager;
	}
}