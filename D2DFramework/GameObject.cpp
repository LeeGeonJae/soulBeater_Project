#include "GameObject.h"
#include "Component.h"

namespace d2dFramework
{
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
}