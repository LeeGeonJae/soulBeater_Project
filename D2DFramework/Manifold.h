#pragma once

#include "Vector2.h"

namespace d2dFramework
{
	struct Manifold
	{
		float Penetration;
		Vector2 CollisionNormal;
	};
}