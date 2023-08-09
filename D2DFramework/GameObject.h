#pragma once

#include "BaseEntity.h"
#include "eObjectType.h"

#include <map>
#include <cassert>
#include <unordered_map>
#include <typeinfo>

namespace d2dFramework
{
	class Component;

	class GameObject : public BaseEntity
	{
		friend class ObjectManager;

	public:
		void Init();
		void Release();

		template <typename T>
		T* CreateComponent(unsigned int id);
		template <typename T>
		T* GetComponent();

		inline void SetObjectType(eObjectType objectType);  // init 호출 전에만 사용해야 합니다. rendering 순서, collision 처리 순서 용도
		inline void SetParent(GameObject* gameObject); // init 호출 전에만 사용해야 합니다. update, fixedUpdate 처리 순서 용도

		inline eObjectType GetObjectType(void) const;
		inline GameObject* GetParentOrNull() const;
		inline unsigned int GetReferenceDepth() const;

	private:
		GameObject(unsigned int id, eObjectType objectType = eObjectType::None);
		~GameObject();

	public:
		enum { MAX_REFERENCE_DEPTH = 3u };

	private:
		eObjectType mObjectType;
		std::unordered_map<size_t, Component*> mComponents;
		GameObject* mParent;
		std::map<unsigned int, GameObject*> mChildren;
		unsigned int mReferenceDepth;
	};

	template <typename T>
	T* GameObject::CreateComponent(unsigned int id)
	{
		bool bIsBase = std::is_base_of<Component, T>::value;
		assert(bIsBase);

		T* component = new T(id, this);
		size_t hash = typeid(T).hash_code();
		mComponents.insert({ hash, component });

		return component;
	}

	template <typename T>
	T* GameObject::GetComponent()
	{
		size_t hash = typeid(T).hash_code();
		auto iter = mComponents.find(hash);

		if (iter == mComponents.end())
		{
			return nullptr;
		}

		T* find = static_cast<T*>((*iter).second);

		return find;
	}

	void GameObject::SetObjectType(eObjectType objectType)
	{
		mObjectType = objectType;
	}
	void GameObject::SetParent(GameObject* gameObject)
	{
		assert(gameObject != nullptr);
		mParent = gameObject;
		mParent->mChildren.insert({ GetId(), this });
		mReferenceDepth = gameObject->GetReferenceDepth() + 1u;

		assert(MAX_REFERENCE_DEPTH > mReferenceDepth);
	}

	eObjectType GameObject::GetObjectType(void) const
	{
		return mObjectType;
	}
	GameObject* GameObject::GetParentOrNull() const
	{
		return mParent;
	}
	unsigned int GameObject::GetReferenceDepth() const
	{
		return mReferenceDepth;
	}
}