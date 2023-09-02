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
	}

	void Scene::Exit()
	{
		// 오브젝트를 삭제 큐에 등록
		ObjectManager::GetInstance()->ClearObjects(false);
		// 삭제 큐에 등록된 오브젝트 즉시 삭제
		ObjectManager::GetInstance()->handleObjectLifeSpan();
		// 각씬마다 타일 아이디가 중복되어서 해당 방식으로 처리, 올바른 방식은 아님
	}

	void Scene::SerializeIn(nlohmann::ordered_json& object)
	{
		for (auto& jsonGameObject : object["m_Gameobjects"])
		{
			unsigned int GameObject_ID = jsonGameObject["GameObject_ID"];
			GameObject* pGameObject = ObjectManager::GetInstance()->CreateObject(GameObject_ID);
			pGameObject->SerializeIn(jsonGameObject);
		}
	}

	void Scene::SerializeOut(nlohmann::ordered_json& object)
	{
		// 수정 : 홍지환, 씬 종속적인 오브젝트는 모두 오브젝트 매니저 Vaild에 저장되도록 로직 수정하여 해당 소스도 변경
		const std::unordered_map<unsigned int, GameObject*>& objectMaps = ObjectManager::GetInstance()->GetObjectMaps();
		object["SceneID"] = this->GetId();

		for (auto& pair : objectMaps)
		{
			nlohmann::ordered_json jsonGameObject;

			GameObject* gameObject = pair.second;
			gameObject->SerializeOut(jsonGameObject);
			object["m_Gameobjects"].push_back(jsonGameObject);
		}
	}
}
