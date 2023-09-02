#include "BoomFlyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "ObjectManager.h"
#include "TestProjectJH.h"
#include "IncludeComponent.h"

#include <cassert>

namespace testProjectJH
{
	BoomFlyComponent::BoomFlyComponent(unsigned int id, d2dFramework::GameObject* owner)
		: Component(id, owner)
		, mProcessedExplosionCount(0u)
	{
	}

	void BoomFlyComponent::Init()
	{
		IUpdateable::Init();
	}

	void BoomFlyComponent::Release()
	{
		IUpdateable::Release();
	}

	void BoomFlyComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		GridComponent* gridComponent = GetGameObject()->GetComponent<GridComponent>();
		GridMoveComponent* girdMoveComponent = GetGameObject()->GetComponent<GridMoveComponent>();
		assert(girdMoveComponent != nullptr);

		GridPosition gridPosition = gridComponent->GetPosition();

		unsigned int distance = MathHelper::GetManhattanDistance(mStartPosition, gridPosition);


		if (gridComponent->GetMaxX() == gridPosition.X + 1
			|| gridPosition.X == 0
			|| gridComponent->GetMaxY() == gridPosition.Y + 1
			|| gridPosition.Y == 0)
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}

		if (distance != 0
			&& distance % mCellCountToExplosion == 0
			&& mProcessedExplosionCount != distance / mCellCountToExplosion)
		{
			createExplosion();
			mProcessedExplosionCount = distance / mCellCountToExplosion;

			if (!mbIsRepeat)
			{
				ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
			}
		}

		if (girdMoveComponent->GetIsCanMove())
		{
			girdMoveComponent->Move(mDirection.X, mDirection.Y);
		}
	}

	void BoomFlyComponent::createExplosion()
	{
		using namespace d2dFramework;

		GridComponent* gridComponent = GetGameObject()->GetComponent<GridComponent>();
		GridMoveComponent* girdMoveComponent = GetGameObject()->GetComponent<GridMoveComponent>();
		assert(girdMoveComponent != nullptr);

		GridPosition gridPosition = gridComponent->GetPosition();

		for (unsigned int i = 0; i < 3; ++i)
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				GameObject* explosion = ObjectManager::GetInstance()->CreateObject(TestProjectJH::GetTempId(), 2.f);
				explosion->CreateComponent<Transform>(TestProjectJH::GetTempId());
				GridComponent* grid = explosion->CreateComponent<GridComponent>(TestProjectJH::GetTempId());
				SpriteRenderer* renderer = explosion->CreateComponent<SpriteRenderer>(TestProjectJH::GetTempId());
				CircleCollider* circleCollider = explosion->CreateComponent<CircleCollider>(TestProjectJH::GetTempId());

				explosion->SetObjectType(eObjectType::Projectile);

				grid->SetMaxX(gridComponent->GetMaxX());
				grid->SetMaxY(gridComponent->GetMaxY());
				grid->SetCellDistance(gridComponent->GetCellDistance());

				GridVector vector = { static_cast<unsigned int>(gridPosition.X + i) - 1, static_cast<unsigned int>(gridPosition.Y + j) - 1 };

				if (vector.X >= gridComponent->GetMaxX()
					|| vector.X < 0
					|| vector.Y >= gridComponent->GetMaxY()
					|| vector.Y < 0)
				{
					continue;
				}

				grid->SetPosition({ static_cast<unsigned int>(vector.X), static_cast<unsigned int>(vector.Y) });

				circleCollider->SetRadius(25);
				renderer->SetSize({ 50, 50 });
				renderer->SetBaseColor({ 1,0,0,0.6 });
				renderer->SetTransformLayer(eTransformLayer::Grid);
			}
		}

	}
}