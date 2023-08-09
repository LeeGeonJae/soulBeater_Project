#include <cmath>
#include <limits>

#include "Vector2.h"

#ifndef EPSILON
#define EPSILON (0.001f)
#endif 

namespace d2dFramework
{
	Vector2::Vector2(float x, float y)
		: D2D_VECTOR_2F{ x, y }
	{
	}

	Vector2::Vector2()
		: Vector2(0.f, 0.f)
	{
	}

	float Vector2::GetDistance(const Vector2& vector, const Vector2& other)
	{
		return sqrtf(GetDistanceSquared(vector, other));
	}

	float Vector2::GetDistanceSquared(const Vector2& vector, const Vector2& other)
	{
		float x = abs(vector.x - other.x);
		float y = abs(vector.y - other.y);

		return (x * x + y * y);
	}

	float Vector2::Dot(const Vector2& vector, const Vector2& other)
	{
		return vector.x * other.x + vector.y * other.y;
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return fabs(x - other.x) < EPSILON && fabs(y - other.y) < EPSILON;
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return fabs(x - other.x) > EPSILON || fabs(y - other.y) > EPSILON;
	}


	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(float scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator*(const D2D1::Matrix3x2F& matrix) const
	{
		D2D1_POINT_2F point = matrix.TransformPoint({ x, y });

		return { point.x ,point.y };
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;

		return *this;
	}

	Vector2& Vector2::operator*=(int scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector2& Vector2::operator*=(float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}

	Vector2& Vector2::operator*=(const D2D1::Matrix3x2F& matrix)
	{
		*this = *this * matrix;

		return *this;
	}

	float Vector2::GetMagnitude() const
	{
		return sqrt(x * x + y * y);
	}

	float Vector2::GetMagnitudeSquared() const
	{
		return (x * x + y * y);
	}

	void Vector2::Normalize()
	{
		float magnitude = GetMagnitude();

		if (magnitude < FLT_EPSILON)
		{
			x = 0;
			y = 0;

			return;
		}

		x /= magnitude;
		y /= magnitude;
	}

	void Vector2::Rotate(float rotateInRadian)
	{
		float cosScalr = cos(rotateInRadian);
		float sinScalr = sin(rotateInRadian);

		Vector2 temp(x * cosScalr + y * sinScalr, x * -sinScalr + y * cosScalr);
		*this = temp;
	}
}

