#pragma once

#include <cassert>

#include "GridPosition.h"
#include "Component.h"
#include "GameObject.h"
#include "ISerialize.h"
#include "Transform.h"
#include "CameraManager.h"
#include "GridMoveComponent.h"

namespace d2dFramework
{
	class GridComponent : public Component
	{
	public:
		GridComponent(unsigned int id, GameObject* owner);
		virtual ~GridComponent() = default;

		virtual void Init() {};
		virtual void Release() {};

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		inline void SetPosition(const GridPosition& position); // maxX, maxY 올바르게 설정후 호출할 것
		inline void SetMaxX(unsigned int maxX);
		inline void SetMaxY(unsigned int maxY);
		inline void SetCellDistance(unsigned int cellDistance);

		inline const GridPosition& GetPosition() const;
		inline unsigned int GetMaxX() const;
		inline unsigned int GetMaxY() const;
		inline unsigned int GetCellDistance() const;

	private:
		GridPosition mPosition;
		unsigned int mMaxX;
		unsigned int mMaxY;
		unsigned int mCellDistance;
	};

	void GridComponent::SetPosition(const GridPosition& position)
	{
		mPosition = position;
		Transform* trnasform = GetGameObject()->GetComponent<Transform>();
		assert(trnasform != nullptr);
		trnasform->SetTranslate({ static_cast<float>(mPosition.X * mCellDistance), static_cast<float>(mPosition.Y * mCellDistance) });

		assert(mPosition.X < mMaxX);
		assert(mPosition.Y < mMaxY);
	}
	void GridComponent::SetMaxX(unsigned int maxX)
	{
		mMaxX = maxX;
	}
	void GridComponent::SetMaxY(unsigned int maxY)
	{
		mMaxY = maxY;
	}
	void GridComponent::SetCellDistance(unsigned int cellDistance)
	{
		mCellDistance = cellDistance;
	}

	const GridPosition& GridComponent::GetPosition() const
	{
		return mPosition;
	}
	unsigned int GridComponent::GetMaxX() const
	{
		return mMaxX;
	}
	unsigned int GridComponent::GetMaxY() const
	{
		return mMaxY;
	}
	unsigned int GridComponent::GetCellDistance() const
	{
		return mCellDistance;
	}

}