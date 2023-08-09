#pragma once

#include <cassert>

namespace d2dFramework
{
	class SceneManager;
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
		inline SceneManager* GetSceneManager() const;

	private:
		static void SetSceneManager(SceneManager* sceneManager);

	private:
		static SceneManager* mSceneManager;
	};

	SceneManager* IUpdateable::GetSceneManager() const
	{
		assert(mSceneManager != nullptr);
		return mSceneManager;
	}
}