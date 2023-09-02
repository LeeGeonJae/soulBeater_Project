#pragma once

#include "BaseEntity.h"

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

		template <typename T>
		T* CreateScene(unsigned int sceneID);
		inline Scene* FindSceneOrNull(unsigned int sceneID) const;
		void DeleteScene(unsigned int sceneID);

		void Init();
		void Release();

		inline void SetCurrentScene(unsigned int sceneID);
		inline Scene* GetCurrentScene() const;

	private:
		Scene* mCurrentScene;
		std::map<unsigned int, Scene*> mSceneMap;
	};

	template <typename T>
	T* SceneManager::CreateScene(unsigned int sceneID)
	{
		auto iter = mSceneMap.find(sceneID);
		assert(iter == mSceneMap.end());

		bool bIsBase = std::is_base_of<Scene, T>::value;
		assert(bIsBase);

		T* scene = new T(sceneID);

		mSceneMap.insert({ sceneID, scene });

		return scene;
	}

	Scene* SceneManager::FindSceneOrNull(unsigned int sceneID) const
	{
		auto iter = mSceneMap.find(sceneID);
		if (iter == mSceneMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}


	void SceneManager::SetCurrentScene(unsigned int sceneID)
	{
		auto finded = mSceneMap.find(sceneID);
		assert(finded != mSceneMap.end());

		mCurrentScene = finded->second;
	}

	Scene* SceneManager::GetCurrentScene() const
	{
		return mCurrentScene;
	}
}