#include "MathHelper.h"

#include "Vector2.h"

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
			size.GetY() * 0.5f + offset.GetY(),
			size.GetX() * 0.5f + offset.GetX(),
			-size.GetY() * 0.5f + offset.GetY()
		};

		return result;
	}
}