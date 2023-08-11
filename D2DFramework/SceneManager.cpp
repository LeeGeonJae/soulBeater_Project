#include "SceneManager.h"

#include "IFixedUpdateable.h"
#include "IUpdateable.h"
#include "eFrameworkID.h"
#include "EventManager.h"
#include "Scene.h"

namespace d2dFramework
{
	SceneManager::SceneManager()
		: BaseEntity(static_cast<unsigned int>(eFrameworkID::SceneManager))
		, mCurrentScene(nullptr)
	{
	}

	SceneManager::~SceneManager()
	{
		for (auto iter = mSceneMap.begin(); iter != mSceneMap.end(); ++iter)
		{
			Scene* scene = iter->second;
			delete scene;
		}

		mSceneMap.clear();
	}

	void SceneManager::Init()
	{
		const std::string& key = EventManager::GetInstance()->GetEventName(eDefaultEvent::ChangeScene);
		auto changeScene = [this](const std::string& data) -> void
		{
			//TODO 이거 잘못들어가면 터짐!!
			const int id = std::stoi(data);

			const auto nextScene = mSceneMap.find(id);
			if (nextScene == mSceneMap.end())
			{
				return;
			}

			mCurrentScene->Exit();
			mCurrentScene = nextScene->second;
			mCurrentScene->Enter();
		};


		EventManager::GetInstance()->RegisterEventHandler(key, BaseEntity::GetId(), changeScene);

	}
	void SceneManager::Release()
	{
		const std::string& key = EventManager::GetInstance()->GetEventName(eDefaultEvent::ChangeScene);
		EventManager::GetInstance()->UnRegisterEventHandler(key, BaseEntity::GetId());

		for (auto iter = mSceneMap.begin(); iter != mSceneMap.end(); ++iter)
		{
			Scene* scene = iter->second;
			scene->Exit();
		}
	}

	Scene* SceneManager::CreateScene(unsigned int sceneID)
	{
		auto iter = mSceneMap.find(sceneID);
		assert(iter == mSceneMap.end());

		Scene* scene = new Scene(sceneID);
		mSceneMap.insert({ sceneID, scene });

		return scene;
	}

	Scene* SceneManager::FindSceneOrNull(unsigned int sceneID)
	{
		auto iter = mSceneMap.find(sceneID);
		if (iter == mSceneMap.end())
		{
			return nullptr;
		}

		return iter->second;
	}


	void SceneManager::FixedUpdate(float deltaTime)
	{
		for (int i = 0; i < GameObject::MAX_REFERENCE_DEPTH; ++i)
		{
			for (auto pair : mFixedUpdateable[i])
			{
				pair.second->FixedUpdate(deltaTime);
			}
		}
	}

	void SceneManager::Update(float deltaTime)
	{
		for (int i = 0; i < GameObject::MAX_REFERENCE_DEPTH; ++i)
		{
			for (auto pair : mUpdateable[i])
			{
				pair.second->Update(deltaTime);
			}
		}
	}


}