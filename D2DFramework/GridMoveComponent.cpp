#include "GridMoveComponent.h"

#include "Transform.h"
#include "GridComponent.h"

namespace d2dFramework
{
	GridMoveComponent::GridMoveComponent(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mOwnerGridComponent(nullptr)
		, mOwnerTransform(nullptr)
		, mObjectSpeed(0.f)
		, mSpeedRatio(0.f)
		, mDelayTime(0.f)
		, mCurrentGridPosition()
		, mNextGirdPosition()
	{
	}
	void GridMoveComponent::Init()
	{
		IUpdateable::Init();

		mOwnerGridComponent = Component::GetGameObject()->GetComponent<GridComponent>();
		mOwnerTransform = Component::GetGameObject()->GetComponent<Transform>();
		mCurrentGridPosition = (mOwnerGridComponent->GetPosition());
		mNextGirdPosition = (mOwnerGridComponent->GetPosition());
	};

	void GridMoveComponent::Release()
	{
		IUpdateable::Release();

		mOwnerGridComponent = nullptr;
		mOwnerTransform = nullptr;
	}

	void GridMoveComponent::Update(float deltaTime)
	{
		// 수정 : 홍지환 그리드 무브먼트 컴포넌트 update는 이동 로직 처리를 의미하므로 현재 위치와 다음위치가 같다면 업데이트 수행 자체가 필요 없음
		if (mCurrentGridPosition == mNextGirdPosition)
		{
			return;
		}
		if (mDelayTime >= 1.f)
		{
			mCurrentGridPosition = mNextGirdPosition;
			mOwnerGridComponent->SetPosition(mCurrentGridPosition);

			return; // early exit
		}

		mDelayTime = mDelayTime + deltaTime * mSpeedRatio;

		float currentXPos = static_cast<float>(mCurrentGridPosition.X * mOwnerGridComponent->GetCellDistance());
		float NextXPos = static_cast<float>(mNextGirdPosition.X * mOwnerGridComponent->GetCellDistance());
		float currentYPos = static_cast<float>(mCurrentGridPosition.Y * mOwnerGridComponent->GetCellDistance());
		float NextYPos = static_cast<float>(mNextGirdPosition.Y * mOwnerGridComponent->GetCellDistance());

		//Percent
		mOwnerTransform->SetTranslate(
			{ MathHelper::Interpolation(currentXPos, NextXPos, mDelayTime * 2)
			, MathHelper::Interpolation(currentYPos, NextYPos, mDelayTime * 2) });
	}

	bool GridMoveComponent::Move(int x, int y)
	{
		// 수정 : 홍지환, 빠른 반환은 else문 들어오지 않으니 if문으로 쓰는 게 더 간결함, 최대값 이동 시도도 빠르게 반환하도록 수정
		if (mCurrentGridPosition.X == 0 && x < 0)
		{
			return false;
		}
		if (mCurrentGridPosition.X == mOwnerGridComponent->GetMaxX() - 1 && x > 0)
		{
			return false;
		}
		if (mCurrentGridPosition.Y == 0 && y < 0)
		{
			return false;
		}
		if (mCurrentGridPosition.Y == mOwnerGridComponent->GetMaxY() - 1 && y > 0)
		{
			return false;
		}

		mNextGirdPosition.X = mCurrentGridPosition.X + x;
		mNextGirdPosition.Y = mCurrentGridPosition.Y + y;
		// 수정 홍지환 : 속도 배율은 속도가 변경되었을 때 같이 처리되는 게 올바름
		// mSpeedRatio = 1.f / mObjectSpeed;
		mDelayTime = 0.f;

		// 추가 이건재 : 다음 위치가 0보다 작거나 MaxX보다 크면 최소값 및 최대값으로 수정
		// unsinged int라 0보다 작아지는 경우에 값이 unsigned int의 최대 값과 비슷해지니 임의의 숫자로 1000000로 설정
		if (mNextGirdPosition.X > 1000000 || mNextGirdPosition.X <= 0)
		{
			mNextGirdPosition.X = 1;
			return false;
		}
		if (mNextGirdPosition.Y > 1000000 || mNextGirdPosition.Y <= 0)
		{
			mNextGirdPosition.Y = 1;
			return false;
		}
		if (mNextGirdPosition.X > mOwnerGridComponent->GetMaxX())
		{
			mNextGirdPosition.X = mOwnerGridComponent->GetMaxX() - 1;
			return false;
		}
		if (mNextGirdPosition.Y > mOwnerGridComponent->GetMaxY())
		{
			mNextGirdPosition.Y = mOwnerGridComponent->GetMaxY() - 1;
			return false;
		}

		return true;
	}

	void GridMoveComponent::MoveNextPositon(const GridPosition& nextPosition)
	{
		mNextGirdPosition = nextPosition;
		mDelayTime = 0.f;

		if (mNextGirdPosition.X >= mOwnerGridComponent->GetMaxX() - 1)
		{
			mNextGirdPosition.X = mOwnerGridComponent->GetMaxX() - 1;
		}
		if (mNextGirdPosition.Y >= mOwnerGridComponent->GetMaxY() - 1)
		{
			mNextGirdPosition.Y = mOwnerGridComponent->GetMaxY() - 1;
		}
	}

	inline void GridMoveComponent::SerializeIn(nlohmann::ordered_json& object)
	{
	}

	inline void GridMoveComponent::SerializeOut(nlohmann::ordered_json& object)
	{
	}
}