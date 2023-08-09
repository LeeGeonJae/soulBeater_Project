#pragma once

#include "BaseEntity.h"
#include "GameObject.h"
#include "IFixedUpdateable.h"
#include "IUpdateable.h"

#include <cassert>
#include <string>
#include <map>
#include <unordered_map>

namespace d2dFramework
{
	class Scene;

	class SceneManager final : public BaseEntity
	{
	public:
		SceneManager();
		~SceneManager();
		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		void Init();
		void FixedUpdate(float deltaTime);
		void Update(float deltaTime);
		void Release();

		void RegisterScene(const std::string& sceneName, Scene* scene);
		inline void RegisterFixedUpdateable(IFixedUpdateable* fixedUpdateable);
		inline void RegisterUpdateable(IUpdateable* updateable);

		inline void UnregisterFixedUpdateable(IFixedUpdateable* fixedUpdateable);
		inline void UnregisterUpdateable(IUpdateable* updateable);

		inline Scene& GetCurrentScene() const;

	private:
		Scene* mCurrentScene;
		std::map<std::string, Scene*> mSceneMap;

		std::unordered_map<unsigned int, IFixedUpdateable*> mFixedUpdateable[GameObject::MAX_REFERENCE_DEPTH];
		std::unordered_map<unsigned int, IUpdateable*> mUpdateable[GameObject::MAX_REFERENCE_DEPTH];
	};

	void SceneManager::RegisterFixedUpdateable(IFixedUpdateable* fixedUpdateable)
	{
		GameObject* gameObject = fixedUpdateable->GetGameObject();
		mFixedUpdateable[gameObject->GetReferenceDepth()].insert({ fixedUpdateable->GetId(), fixedUpdateable });
	}
	void SceneManager::RegisterUpdateable(IUpdateable* updateable)
	{
		GameObject* gameObject = updateable->GetGameObject();
		mUpdateable[gameObject->GetReferenceDepth()].insert({ updateable->GetId(), updateable });
	}

	void SceneManager::UnregisterFixedUpdateable(IFixedUpdateable* fixedUpdateable)
	{
		GameObject* gameObject = fixedUpdateable->GetGameObject();
		mFixedUpdateable[gameObject->GetReferenceDepth()].erase(mFixedUpdateable->find(fixedUpdateable->GetId()));
	}
	void SceneManager::UnregisterUpdateable(IUpdateable* updateable)
	{
		GameObject* gameObject = updateable->GetGameObject();
		mUpdateable[gameObject->GetReferenceDepth()].erase(mUpdateable->find(updateable->GetId()));
	}

	Scene& SceneManager::GetCurrentScene() const
	{
		return *mCurrentScene;
	}
}