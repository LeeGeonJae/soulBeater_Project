#include "FlyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "ObjectManager.h"
#include "CircleCollider.h"

#include <cassert>

namespace soulBeater
{
	FlyComponent::FlyComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component(id, owner)
	{
	}

	void FlyComponent::Init()
	{
		using namespace d2dFramework;

		IUpdateable::Init();

		d2dFramework::GameObject* object = GetGameObject();
		CircleCollider* collider = object->GetComponent<CircleCollider>();

		auto beginoverlap = [this](unsigned int wallid)
		{
			GameObject* wall = ObjectManager::GetInstance()->FindObjectOrNull(wallid);

			if (wall->GetObjectType() == eObjectType::Wall)
			{
				assert(wall != nullptr);

				ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
			}
		};

		collider->RegisterOnEnterCollisionCallBack(GetGameObject()->GetId(), beginoverlap);
	}

	void FlyComponent::Release()
	{
		IUpdateable::Release();
	}

	void FlyComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GridComponent* gridComponent = GetGameObject()->GetComponent<GridComponent>();
		GridMoveComponent* girdMoveComponent = GetGameObject()->GetComponent<GridMoveComponent>();
		assert(girdMoveComponent != nullptr);

		if (gridComponent->GetMaxX() == gridComponent->GetPosition().X
			|| gridComponent->GetPosition().X == 1
			|| gridComponent->GetMaxY() == gridComponent->GetPosition().Y
			|| gridComponent->GetPosition().Y == 0)
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}

		if (girdMoveComponent->GetIsCanMove())
		{
			girdMoveComponent->Move(-1, 0);
		}
	}
}