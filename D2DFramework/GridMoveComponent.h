#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "MathHelper.h"
#include "GridPosition.h"

namespace d2dFramework
{
	class Transform;
	class GridComponent;
	class GameObject;

	class GridMoveComponent final : public Component, public IUpdateable
	{
	public:
		GridMoveComponent(unsigned int id, GameObject* owner);
		virtual ~GridMoveComponent() = default;

		void Init();
		void Update(float deltaTime);
		void Release();

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		bool Move(int x, int y);
		void MoveNextPositon(const GridPosition& nextPosition);

		inline void SetCurrentSpeed(float objectspeed);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline const GridPosition& GetCurrentPosition() const;
		inline const GridPosition& GetNextPosition() const;
		inline float GetCurrentSpeed();
		inline bool GetIsCanMove();

	private:
		GridComponent* mOwnerGridComponent;
		Transform* mOwnerTransform;
		GridPosition mCurrentGridPosition;
		GridPosition mNextGirdPosition;

		float mDelayTime;
		float mObjectSpeed;
		float mSpeedRatio;
	};

	GameObject* GridMoveComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	const GridPosition& GridMoveComponent::GetCurrentPosition() const
	{
		return mCurrentGridPosition;
	}
	const GridPosition& GridMoveComponent::GetNextPosition() const
	{
		return mNextGirdPosition;
	}
	unsigned int GridMoveComponent::GetId() const
	{
		return BaseEntity::GetId();
	}

	bool GridMoveComponent::GetIsCanMove()
	{
		return mCurrentGridPosition == mNextGirdPosition;
	}

	void GridMoveComponent::SetCurrentSpeed(float objectspeed)
	{
		mObjectSpeed = objectspeed;
		mSpeedRatio = 1.f / mObjectSpeed;
	}

	float GridMoveComponent::GetCurrentSpeed()
	{
		return mObjectSpeed;
	}
}