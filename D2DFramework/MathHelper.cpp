#include "MathHelper.h"

#include "Vector2.h"

#include <cassert>

#define PI_F (3.141592f)

namespace d2dFramework
{
	float MathHelper::RadianToDegree(float radian)
	{
		return radian * 180.f / PI_F;
	}

	float MathHelper::DegreeToRadian(float degree)
	{
		return degree * PI_F / 180.f;
	}

	float MathHelper::Clamp(float num, float min, float max)
	{
		if (num > max)
		{
			return max;
		}
		else if (num < min)
		{
			return min;
		}

		return num;
	}

	D2D1_RECT_F MathHelper::CreateRectangle(const Vector2& offset, const Vector2& size)
	{
		D2D1_RECT_F result =
		{
			-size.GetX() * 0.5f + offset.GetX(),
			-size.GetY() * 0.5f + offset.GetY(),
			size.GetX() * 0.5f + offset.GetX(),
			size.GetY() * 0.5f + offset.GetY()
		};

		return result;
	}

	double MathHelper::GetDoubleRemainder(double dividend, double divisor) {
		return dividend - (divisor * static_cast<int>(dividend / divisor));
	}

	float MathHelper::Interpolation(float start, float end, float normalizedDistance)
	{
		normalizedDistance = Clamp(normalizedDistance, 0.f, 1.f);

		return (1 - normalizedDistance) * start + normalizedDistance * end;
	}

	GridVector MathHelper::SubtractGirdPosition(const GridPosition& lhs, const GridPosition& rhs)
	{
		return { static_cast<int>(lhs.X) - static_cast<int>(rhs.X), static_cast<int>(lhs.Y) - static_cast<int>(rhs.Y) };
	}

	unsigned int MathHelper::GetManhattanDistance(const GridPosition& lhs, const GridPosition& rhs)
	{
		int x = lhs.X - rhs.X;
		int y = lhs.Y - rhs.Y;

		if (x < 0)
		{
			x *= -1;
		}
		if (y < 0)
		{
			y *= -1;
		}
		assert(x >= 0);
		assert(y >= 0);

		return x + y;
	}

	unsigned int MathHelper::GetAxisMaxDistance(const GridPosition& lhs, const GridPosition& rhs)
	{
		int x = lhs.X - rhs.X;
		int y = lhs.Y - rhs.Y;

		if (x < 0)
		{
			x *= -1;
		}
		if (y < 0)
		{
			y *= -1;
		}
		assert(x >= 0);
		assert(y >= 0);

		return x > y ? x : y;
	}
}