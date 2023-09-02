#pragma once

#include <d2d1helper.h>

namespace d2dFramework
{
	class Vector2 : private D2D_VECTOR_2F
	{
	public:
		Vector2();
		~Vector2() = default;
		Vector2(const Vector2&) = default;
		Vector2& operator=(const Vector2&) = default;
		Vector2(float x, float y);

		static float GetDistance(const Vector2& vector, const Vector2& other);
		static float GetDistanceSquared(const Vector2& vector, const Vector2& other);
		static float Dot(const Vector2& vector, const Vector2& other);

		// operator 
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator*(float scalar) const;
		Vector2 operator/(float scalar) const;
		Vector2 operator*(const D2D1::Matrix3x2F& matrix) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator*=(int scalar);
		Vector2& operator*=(float scalar);
		Vector2& operator*=(const D2D1::Matrix3x2F& matrix);
		inline Vector2 operator-();

		void Normalize();
		void Rotate(float rotateInRadian);
		inline void Move(float x, float y);
		inline void SetX(float x);
		inline void SetY(float y);
		inline void SetXY(float x, float y);
		inline void AbsX();
		inline void AbsY();
		inline void AbsXY();

		float GetMagnitude() const;
		float GetMagnitudeSquared() const;
		inline float GetX() const;
		inline float GetY() const;
	};

	Vector2 Vector2::operator-()
	{
		return Vector2(-x, -y);
	}

	float Vector2::GetX() const
	{
		return x;
	}

	float Vector2::GetY() const
	{
		return y;
	}

	void Vector2::SetX(float x)
	{
		this->x = x;
	}

	void Vector2::SetY(float y)
	{
		this->y = y;
	}

	void Vector2::SetXY(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	void Vector2::AbsX()
	{
		if (this->x < 0.f)
		{
			this->x = -this->x;
		}
	}
	void Vector2::AbsY()
	{
		if (this->y < 0.f)
		{
			this->y = -this->y;
		}
	}
	void Vector2::AbsXY()
	{
		AbsX();
		AbsY();
	}

	void Vector2::Move(float x, float y)
	{
		this->x += x;
		this->y += y;
	}
}