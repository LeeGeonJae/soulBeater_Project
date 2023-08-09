#pragma once

#include <cassert>

#include "Vector2.h"

namespace d2dFramework
{
	enum class eRectangleIndex
	{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Size
	};

	class hRectangle
	{
	public:
		hRectangle();
		~hRectangle() = default;
		hRectangle(const hRectangle&) = default;
		hRectangle& operator=(const hRectangle&) = default;

		hRectangle(Vector2 topLeft, Vector2 bottomRight);
		hRectangle(float x1, float y1, float x2, float y2);

		hRectangle operator*(const D2D1::Matrix3x2F& matrix) const;
		hRectangle& operator*=(const D2D1::Matrix3x2F& matrix);

		void Rotate(float originX, float originY, float cosf, float sinf);
		void Rotate(float originX, float originY, float radian);
		inline void Move(float x, float y);

		inline void SetTopLeft(const Vector2& topLeft);
		inline void SetTopRight(const Vector2& bottomRight);
		inline void SetBottomLeft(const Vector2& bottomRight);
		inline void SetBottomRight(const Vector2& bottomRight);
		inline void SetVertex(eRectangleIndex rectangleIndex, const Vector2 vertex);

		inline const Vector2& GetVertex(eRectangleIndex rectangleIndex) const;
		inline const Vector2& GetTopLeft() const;
		inline const Vector2& GetTopRight() const;
		inline const Vector2& GetBottomLeft() const;
		inline const Vector2& GetBottomRight() const;
		inline Vector2 GetCenter() const;
		inline Vector2 GetSize() const;

		// 똑바로 된 사각형만 사용할 수 있다.
		inline float GetWidth() const;
		inline float GetHeight() const;
		static bool IsCollision(const hRectangle& rect, const hRectangle& otherRect);
		static bool IsContained(const hRectangle& rect, const hRectangle& otherRect);
		static bool IsContainVertex(const hRectangle& rect, float x, float y);
		static hRectangle GetIntersection(const hRectangle& rect, const hRectangle& otherRect);

		static hRectangle GetBoundingRectangle(const hRectangle& rectangle);

	private:
		inline Vector2& getVertex(eRectangleIndex rectangleIndex);

	private:
		Vector2 mVertices[static_cast<size_t>(eRectangleIndex::Size)];
	};

	void hRectangle::Move(float x, float y)
	{
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i].Move(x, y);
		}
	}

	void hRectangle::SetTopLeft(const Vector2& topLeft)
	{
		getVertex(eRectangleIndex::TopLeft) = topLeft;
	}

	void hRectangle::SetTopRight(const Vector2& topRight)
	{
		getVertex(eRectangleIndex::TopRight) = topRight;
	}

	void hRectangle::SetBottomLeft(const Vector2& bottomLeft)
	{
		getVertex(eRectangleIndex::BottomLeft) = bottomLeft;
	}

	void hRectangle::SetBottomRight(const Vector2& bottomRight)
	{
		getVertex(eRectangleIndex::BottomRight) = bottomRight;
	}

	void hRectangle::SetVertex(eRectangleIndex rectangleIndex, const Vector2 vertex)
	{
		getVertex(rectangleIndex) = vertex;
	}

	const Vector2& hRectangle::GetTopLeft() const
	{
		return GetVertex(eRectangleIndex::TopLeft);
	}

	const Vector2& hRectangle::GetTopRight() const
	{
		return GetVertex(eRectangleIndex::TopRight);
	}

	const Vector2& hRectangle::GetBottomLeft() const
	{
		return GetVertex(eRectangleIndex::BottomLeft);
	}

	const Vector2& hRectangle::GetBottomRight() const
	{
		return GetVertex(eRectangleIndex::BottomRight);
	}

	Vector2 hRectangle::GetCenter() const
	{
		const Vector2& topLeft = GetVertex(eRectangleIndex::TopLeft);
		const Vector2& bottomRight = GetVertex(eRectangleIndex::BottomRight);

		return Vector2((topLeft.GetX() + bottomRight.GetX()) * 0.5f, (topLeft.GetY() + bottomRight.GetY()) * 0.5f);
	}

	Vector2 hRectangle::GetSize() const
	{
		return Vector2(GetWidth(), GetHeight());
	}

	float hRectangle::GetWidth() const
	{
		return GetVertex(eRectangleIndex::TopRight).GetX() - GetVertex(eRectangleIndex::TopLeft).GetX();
	}

	float hRectangle::GetHeight() const
	{
		return GetVertex(eRectangleIndex::BottomLeft).GetY() - GetVertex(eRectangleIndex::TopLeft).GetY();
	}

	Vector2& hRectangle::getVertex(eRectangleIndex rectangleIndex)
	{
		assert(rectangleIndex != eRectangleIndex::Size);

		return mVertices[static_cast<size_t>(rectangleIndex)];
	}

	inline const Vector2& hRectangle::GetVertex(eRectangleIndex rectangleIndex) const
	{
		assert(rectangleIndex != eRectangleIndex::Size);

		return mVertices[static_cast<size_t>(rectangleIndex)];
	}
}