#include "GridComponent.h"

namespace d2dFramework
{
	GridComponent::GridComponent(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mPosition{ 0, 0 }
		, mMaxX(0)
		, mMaxY(0)
		, mCellDistance(0)
	{
	}

	void GridComponent::SerializeIn(nlohmann::ordered_json& object)
	{
		mPosition.X = object["GridPosition"][0];
		mPosition.Y = object["GridPosition"][1];
		mMaxX = object["MaxX"];
		mMaxY = object["MaxY"];
		mCellDistance = object["CellDistance"];
	}

	void GridComponent::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "GridComponent";
		Component::SerializeOut(object);
		object["GridPosition"] = { mPosition.X, mPosition.Y };
		object["MaxX"] = mMaxX;
		object["MaxY"] = mMaxY;
		object["CellDistance"] = mCellDistance;
	}
}