#include "BoomFlyComponent.h"

#include "GameObject.h"
#include "GridComponent.h"
#include "GridMoveComponent.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "SoulBeaterProcessor.h"
#include "IncludeComponent.h"
#include "ConstantTable.h"

#include <cassert>

namespace soulBeater
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
		GridPosition nextGridPosition = girdMoveComponent->GetNextPosition();

		unsigned int distance = MathHelper::GetManhattanDistance(mStartPosition, gridPosition);

		if (gridComponent->GetMaxX() <= nextGridPosition.X
			|| gridComponent->GetMaxY() <= nextGridPosition.Y)
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}

		if (mCellCountToExplosion == 0)
		{
			createExplosion();
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
			return;
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
			if (!girdMoveComponent->Move(mDirection.X, mDirection.Y))
			{
				ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
			}
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
				GameObject* explosion = ObjectManager::GetInstance()->CreateObject(SoulBeaterProcessor::GetTempId(), 0.34f);
				explosion->CreateComponent<Transform>(SoulBeaterProcessor::GetTempId());
				GridComponent* grid = explosion->CreateComponent<GridComponent>(SoulBeaterProcessor::GetTempId());
				AnimationRenderer* renderer = explosion->CreateComponent<AnimationRenderer>(SoulBeaterProcessor::GetTempId());
				CircleCollider* circleCollider = explosion->CreateComponent<CircleCollider>(SoulBeaterProcessor::GetTempId());

				explosion->SetObjectType(eObjectType::Projectile);

				grid->SetMaxX(gridComponent->GetMaxX());
				grid->SetMaxY(gridComponent->GetMaxY());
				grid->SetCellDistance(gridComponent->GetCellDistance());

				GridVector vector = { static_cast<int>(gridPosition.X + i) - 1, static_cast<int>(gridPosition.Y + j) - 1 };

				if (static_cast<unsigned int>(vector.X) >= gridComponent->GetMaxX()
					|| vector.X < 0
					|| static_cast<unsigned int>(vector.Y) >= gridComponent->GetMaxY()
					|| vector.Y < 0)
				{
					continue;
				}

				grid->SetPosition({ static_cast<unsigned int>(vector.X), static_cast<unsigned int>(vector.Y) });

				circleCollider->SetRadius(25);
				renderer->SetSize({ static_cast<float>(GRID_DISTANCE), static_cast<float>(GRID_DISTANCE) });
				renderer->SetAnimationAsset(RenderManager::GetInstance()->GetAnimationAssetOrNull(BOOM_ANI_KEY));
				renderer->SetTransformLayer(eTransformLayer::Grid);
			}
		}

	}
}