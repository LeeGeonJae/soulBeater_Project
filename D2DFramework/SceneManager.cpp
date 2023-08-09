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
			auto nextScene = mSceneMap.find(data);
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

	void SceneManager::RegisterScene(const std::string& sceneName, Scene* scene)
	{
		if (mCurrentScene == nullptr)
		{
			mCurrentScene = scene;
			mCurrentScene->Enter();
		}

		auto iter = mSceneMap.find(sceneName);
		assert(iter == mSceneMap.end()); // 동일한 이름의 씬을 등록하면 안댐

		mSceneMap.insert({ sceneName, scene });
	}
}