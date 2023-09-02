#include "SceneManager.h"

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


		EventManager::GetInstance()->RegisterEventHandler("ChangeScene", BaseEntity::GetId(), changeScene);

	}
	void SceneManager::Release()
	{
		EventManager::GetInstance()->UnregisterEventHandler("ChangeScene", BaseEntity::GetId());

		for (auto iter = mSceneMap.begin(); iter != mSceneMap.end(); ++iter)
		{
			Scene* scene = iter->second;
			scene->Exit();
		}
	}

	void SceneManager::DeleteScene(unsigned int sceneID)
	{
		auto iter = mSceneMap.find(sceneID);
		delete iter->second;
		mSceneMap.erase(sceneID);
	}
}