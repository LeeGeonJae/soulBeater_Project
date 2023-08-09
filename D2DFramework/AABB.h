#pragma once

#include <d2d1.h>
#include "Vector2.h"

namespace d2dFramework
{
	struct AABB
	{
		Vector2 TopLeft;
		Vector2 BottomRight;
	};
}