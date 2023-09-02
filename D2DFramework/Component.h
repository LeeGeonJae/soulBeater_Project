#pragma once

#include "BaseEntity.h"
#include "ISerialize.h"

namespace d2dFramework
{
	class GameObject;

	class Component : public BaseEntity, public ISerialize
	{
	public:
		Component(unsigned int id, GameObject* owner);
		virtual ~Component() = default;

		virtual void Init() = 0;
		virtual void Release() = 0;
		virtual void SerializeOut(nlohmann::ordered_json& object) override;
		virtual void SerializeIn(nlohmann::ordered_json& object) = 0;
	protected:
		inline GameObject* GetGameObject() const; // 다른 인터페이스에서 함수 참조하기 위해서 virtual로 선언

	private:
		GameObject* mOwner;
	};

	GameObject* Component::GetGameObject() const
	{
		return mOwner;
	}
}
