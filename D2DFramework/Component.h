#pragma once

#include "BaseEntity.h"

namespace d2dFramework
{
	class GameObject;

	class Component : public BaseEntity
	{
	public:
		Component(unsigned int id, GameObject* owner);
		virtual ~Component() = default;

		virtual void Init() = 0;
		virtual void Release() = 0;

	protected:
		inline GameObject* GetGameObject() const; // �ٸ� �������̽����� �Լ� �����ϱ� ���ؼ� virtual�� ����

	private:
		GameObject* mOwner;
	};

	GameObject* Component::GetGameObject() const
	{
		return mOwner;
	}
}