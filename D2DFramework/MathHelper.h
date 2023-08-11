#pragma once

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
	};
}