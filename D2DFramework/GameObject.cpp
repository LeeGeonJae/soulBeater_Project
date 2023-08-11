#include "GameObject.h"

#include "AABBCollider.h"
#include "AnimationRenderer.h"
#include "CircleCollider.h"
#include "Component.h"
#include "OBBCollider.h"
#include "ObjectManager.h"
#include "Rigidbody.h"
#include "SpriteRenderer.h"
#include "Transform.h"

namespace d2dFramework
{
	class Transform;
	class SpriteRenderer;
	class Rigidbody;
	class OBBCollider;
	class AnimationRenderer;
	class AABBCollider;

	GameObject::GameObject(unsigned int id, eObjectType objectType)
		: BaseEntity(id)
		, mObjectType(objectType)
		, mParent(nullptr)
		, mReferenceDepth(0u)
	{
	}

	GameObject::~GameObject()
	{
		for (auto pair : mComponents)
		{
			delete pair.second;
		}
	}

	void GameObject::Init()
	{
		for (auto keyComponent : mComponents)
		{
			Component* component = keyComponent.second;
			component->Init();
		}
	}

	void GameObject::Release()
	{
		if (mParent != nullptr)
		{
			mParent->mChildren.erase(GetId());
		}

		for (auto pair : mChildren)
		{
			pair.second->Release();
		}

		for (auto keyComponent : mComponents)
		{
			keyComponent.second->Release();
		}
	}

	void GameObject::SerializeIn(nlohmann::ordered_json& object)
	{
		mObjectType = object["mObjectType"];
		mReferenceDepth = object["mReferenceDepth"];
		for(auto& JsonFile: object["mComponents"])
		{
			std::string _ComponentName = JsonFile["ComponentName"];
			int component_ID = JsonFile["Component_ID"];
			if (_ComponentName == "AABBCollider") 
				CreateComponent<AABBCollider>(component_ID)->SerializeIn(JsonFile);	//고유값 넣어주기
				//GetComponent<AABBCollider>()->SerializeIn(JsonFile);
			else if (_ComponentName == "AnimationRenderer") 
				CreateComponent<AnimationRenderer>(component_ID)->SerializeIn(JsonFile);//고유값 넣어주기
			else if (_ComponentName == "CircleCollider") 
				CreateComponent<CircleCollider>(component_ID)->SerializeIn(JsonFile);//고유값 넣어주기
			else if (_ComponentName == "OBBCollider")
				CreateComponent<OBBCollider>(component_ID)->SerializeIn(JsonFile);//고유값 넣어주기
			else if (_ComponentName == "Rigidbody")
				CreateComponent<Rigidbody>(component_ID)->SerializeIn(JsonFile);
			else if (_ComponentName == "SpriteRenderer")
				CreateComponent<SpriteRenderer>(component_ID)->SerializeIn(JsonFile);
			else if (_ComponentName == "Transform")
				CreateComponent<Transform>(component_ID)->SerializeIn(JsonFile);


			if (object.contains("mChildren"))
			{
				for (auto& test_Gameobjects : object["mChildren"])
				{
					unsigned int ChildGameObject_ID = test_Gameobjects["GameObject_ID"];
					GameObject* pChildGameObject = ObjectManager::GetInstance()->CreateObject(ChildGameObject_ID);
					pChildGameObject->SerializeIn(test_Gameobjects);
					pChildGameObject->SetParent(this);
				}
			}

		}

		///children시스템 구현 생각해보기....

		//for (auto& JsonFile : object["mChildren"])
		//{
		//	std::string _GameObjectName = JsonFile["GameObject_ID"];

		//}





	}

	void GameObject::SerializeOut(nlohmann::ordered_json& object)
	{
		object["GameObject_ID"] = this->GetId();
		object["mObjectType"] = (int)mObjectType;
		object["mReferenceDepth"] = mReferenceDepth;
		for (auto& pComponent : mComponents)
		{
			nlohmann::ordered_json JsonComponents;
			pComponent.second->SerializeOut(JsonComponents);
			object["mComponents"].push_back(JsonComponents);
		}
	}
}
