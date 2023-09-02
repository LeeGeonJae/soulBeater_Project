#include "FlyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "ObjectManager.h"

#include <cassert>

namespace testProjectJH
{
	FlyComponent::FlyComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component(id, owner)
	{
	}

	void FlyComponent::Init()
	{
		IUpdateable::Init();
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

		if (gridComponent->GetMaxX() == gridComponent->GetPosition().X + 1
			|| gridComponent->GetPosition().X == 0
			|| gridComponent->GetMaxY() == gridComponent->GetPosition().Y + 1
			|| gridComponent->GetPosition().Y == 0)
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}

		if (girdMoveComponent->GetIsCanMove())
		{
			girdMoveComponent->Move(mDirection.X, mDirection.Y);
		}
	}
}