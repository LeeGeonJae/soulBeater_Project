#pragma once

#include "Vector2.h"

namespace d2dFramework
{
	enum eOBBIndex
	{
		TopLeft,
		TopRight,
		BottomRight,
		BottomLeft,
		Size
	};

	struct OBB
	{
		Vector2 mPoints[eOBBIndex::Size];
		float RotateInRadian;
	};
}