#include "Scene.h"

#include "GameObject.h"
#include "ICollideable.h"
#include "IFixedUpdateable.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "SceneLoader.h"
#include "EventManager.h"

namespace d2dFramework
{
	Scene::Scene(unsigned int id)
		: BaseEntity(id)
	{
	}

	void Scene::Enter()
	{
		SceneLoader::LoadScene(this);
		GameObject* gameObject = CreateObject(1);

	}

	void Scene::Exit()
	{
		EventManager::GetInstance()->BroadcastEvent("SceneChange", "mName");
	}

	GameObject* Scene::CreateObject(unsigned int id)
	{
		GameObject* gameObject= ObjectManager::GetInstance()->CreateObject(id);
		mObjectIDs.insert(id);
		return gameObject;
	}

	void Scene::SerializeIn(nlohmann::ordered_json& object)
	{
		this->mGridWidth = object["GridWidth"];
		this->mGridHeight = object["GridHeight"];
		this->mGridDistance = object["GridDistance"];

		for (auto& jsonGameObject : object["m_Gameobjects"])
		{
			unsigned int GameObject_ID = jsonGameObject["GameObject_ID"];
			GameObject* pGameObject = this->CreateObject(GameObject_ID);
			pGameObject->SerializeIn(jsonGameObject);

			///child설정... 까다로워서 일단 대기
		/*	if (jsonGameObject.contains("mChildren"))
			{
				for (auto& test_Gameobjects : jsonGameObject["mChildren"])
				{
					unsigned int ChildGameObject_ID = test_Gameobjects["GameObject_ID"];
					GameObject* pChildGameObject = outScene->CreateObject(ChildGameObject_ID);
					pChildGameObject->SerializeIn(test_Gameobjects);
					pChildGameObject->SetParent(pGameObject);
				}
			}*/
		}
		
	}

	void Scene::SerializeOut(nlohmann::ordered_json& object)
	{
		const std::unordered_map<unsigned int, GameObject*>& objects = ObjectManager::GetInstance()->GetObjects();
		object["SceneID"] = this->GetId();
		object["GridWidth"] = this->mGridWidth;
		object["GridHeight"] = this->mGridHeight;
		object["GridDistance"]=this->mGridDistance;
	

		for (unsigned int gameObjectID : this->mObjectIDs)
		{
			nlohmann::ordered_json jsonGameObject;
			if (objects.find(gameObjectID) != objects.end())
			{
				objects.find(gameObjectID)->second->SerializeOut(jsonGameObject);
				object["m_Gameobjects"].push_back(jsonGameObject);
			}
			else
			{
				///TODO 터뜨리는 오류코드 필요.
			}

		}


	}
		
}
