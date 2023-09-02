#include "FastFlyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "ObjectManager.h"

#include <cassert>

namespace testProjectJH
{
	FastFlyComponent::FastFlyComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component(id, owner)
	{
	}

	void FastFlyComponent::Init()
	{
		IUpdateable::Init();
	}

	void FastFlyComponent::Release()
	{
		IUpdateable::Release();
	}

	void FastFlyComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		if (mWarningDuration > 0)
		{
			mWarningDuration -= deltaTime;
			// debugRender
			return;
		}

		GridComponent* gridComponent = GetGameObject()->GetComponent<GridComponent>();
		GridMoveComponent* girdMoveComponent = GetGameObject()->GetComponent<GridMoveComponent>();
		assert(girdMoveComponent != nullptr);

		if (girdMoveComponent->GetIsCanMove())
		{
			if (mbIsColumn)
			{
				girdMoveComponent->MoveNextPositon({ 0, mStartPosition.Y });
			}
			else
			{
				girdMoveComponent->MoveNextPositon({ mStartPosition.X , gridComponent->GetMaxY() - 1 });
			}
		}

		if (girdMoveComponent->GetIsCanMove())
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}
	}
}