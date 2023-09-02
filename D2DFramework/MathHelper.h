#pragma once

#include "GridPosition.h"
#include "GridVector.h"

#include <d2d1.h>

namespace d2dFramework
{
	class Vector2;

	class MathHelper
	{
	public:
		static float RadianToDegree(float radian);
		static float DegreeToRadian(float degree);
		static float Clamp(float num, float min, float max);
		static D2D1_RECT_F CreateRectangle(const Vector2& offset, const Vector2& size);
		static double GetDoubleRemainder(double dividend, double divisor);
		static float Interpolation(float start, float end, float normalizedDistance); // normalizeDistance 0~1, 넘어간 값은 클램핑 처리
		static GridVector SubtractGirdPosition(const GridPosition& lhs, const GridPosition& rhs);
		static unsigned int GetManhattanDistance(const GridPosition& lhs, const GridPosition& rhs);
		static unsigned int GetAxisMaxDistance(const GridPosition& lhs, const GridPosition& rhs);
	};
}