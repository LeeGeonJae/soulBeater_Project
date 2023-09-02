#include <cassert>
#include <cmath>

#include "hRectangle.h"

namespace d2dFramework
{
	hRectangle::hRectangle(Vector2 topLeft, Vector2 bottomRight)
	{
		Vector2& memTopLeft = getVertex(eRectangleIndex::TopLeft);
		Vector2& memBottomRight = getVertex(eRectangleIndex::BottomRight);

		memTopLeft = topLeft;
		memBottomRight = bottomRight;

		/*float temp;
		if (getVertex(eRectangleIndex::TopLeft).GetX() > getVertex(eRectangleIndex::BottomRight).GetX())
		{
			temp = memTopLeft.GetX();
			memTopLeft.SetX(memBottomRight.GetX());
			memBottomRight.SetX(temp);
		}
		if (memTopLeft.GetY() > memBottomRight.GetY())
		{
			temp = memTopLeft.GetY();
			memTopLeft.SetY(memBottomRight.GetY());
			memBottomRight.SetY(temp);
		}*/

		getVertex(eRectangleIndex::TopRight) = Vector2(memBottomRight.GetX(), memTopLeft.GetY());
		getVertex(eRectangleIndex::BottomLeft) = Vector2(memTopLeft.GetX(), memBottomRight.GetY());
	}

	hRectangle::hRectangle(float x1, float y1, float x2, float y2)
		:hRectangle(Vector2(x1, y1), Vector2(x2, y2))
	{
	}

	hRectangle::hRectangle()
		: hRectangle(0.f, 0.f, 0.f, 0.f)
	{
	}

	hRectangle hRectangle::operator*(const D2D1::Matrix3x2F& matrix) const
	{
		hRectangle result(*this);

		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			result.mVertices[i] *= matrix;
		}

		return result;
	}

	hRectangle& hRectangle::operator*=(const D2D1::Matrix3x2F& matrix)
	{
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i] *= matrix;
		}

		return *this;
	}

	void hRectangle::Rotate(float originX, float originY, float cosf, float sinf)
	{
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i].Move(-originX, -originY);
		}

		// 회전 
		{
			Vector2 temp;
			for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
			{
				temp.SetX(mVertices[i].GetX() * cosf - mVertices[i].GetY() * sinf);
				temp.SetY(mVertices[i].GetX() * sinf + mVertices[i].GetY() * cosf);
				mVertices[i] = temp;
			}
		}

		// 원점 초기화
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i].Move(originX, originY);
		}
	}

	void hRectangle::Rotate(float originX, float originY, float radian)
	{
		// 원점 옮기기
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i].Move(-originX, -originY);
		}

		// 회전 
		{
			float cosScalr = cos(radian);
			float sinScalr = sin(radian);
			Vector2 temp;
			for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
			{
				temp.SetX(mVertices[i].GetX() * cosScalr - mVertices[i].GetY() * sinScalr);
				temp.SetY(mVertices[i].GetX() * sinScalr + mVertices[i].GetY() * cosScalr);
				mVertices[i] = temp;
			}
		}

		// 원점 초기화
		for (size_t i = 0; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			mVertices[i].Move(originX, originY);
		}
	}

	bool hRectangle::IsCollision(const hRectangle& rect, const hRectangle& otherRect)
	{
		return rect.GetBottomRight().GetX() >= otherRect.GetTopLeft().GetX()
			&& otherRect.GetBottomRight().GetX() >= rect.GetTopLeft().GetX()
			&& rect.GetTopLeft().GetY() <= otherRect.GetBottomRight().GetY()
			&& otherRect.GetTopLeft().GetY() <= rect.GetBottomRight().GetY();
	}

	bool hRectangle::IsContained(const hRectangle& rect, const hRectangle& otherRect)
	{
		return rect.GetTopLeft().GetX() < otherRect.GetTopLeft().GetX()
			&& rect.GetTopLeft().GetY() < otherRect.GetTopLeft().GetY()
			&& rect.GetBottomRight().GetX() > otherRect.GetBottomRight().GetX()
			&& rect.GetBottomRight().GetY() > otherRect.GetBottomRight().GetY();
	}

	bool hRectangle::IsContainVertex(const hRectangle& rect, float x, float y)
	{
		return rect.GetTopLeft().GetX() < x
			&& rect.GetTopLeft().GetY() < y
			&& rect.GetBottomRight().GetX() > x
			&& rect.GetBottomRight().GetY() > y;
	}

	hRectangle hRectangle::GetIntersection(const hRectangle& rect, const hRectangle& otherRect)
	{
		IsCollision(rect, otherRect);

		Vector2 interSectionTopLeft(
			rect.GetTopLeft().GetX() > otherRect.GetTopLeft().GetX() ? rect.GetTopLeft().GetX() : otherRect.GetTopLeft().GetX(),
			rect.GetTopLeft().GetY() > otherRect.GetTopLeft().GetY() ? rect.GetTopLeft().GetY() : otherRect.GetTopLeft().GetY());
		Vector2 interSectionBottomRight(
			rect.GetBottomRight().GetX() < otherRect.GetBottomRight().GetX() ? rect.GetBottomRight().GetX() : otherRect.GetBottomRight().GetX(),
			rect.GetBottomRight().GetY() < otherRect.GetBottomRight().GetY() ? rect.GetBottomRight().GetY() : otherRect.GetBottomRight().GetY());

		return hRectangle(interSectionTopLeft, interSectionBottomRight);
	}

	hRectangle hRectangle::GetBoundingRectangle(const hRectangle& rectangle)
	{
		float minX = rectangle.mVertices[0].GetX();
		float minY = rectangle.mVertices[0].GetY();
		float maxX = rectangle.mVertices[0].GetX();
		float maxY = rectangle.mVertices[0].GetY();

		for (size_t i = 1; i < static_cast<size_t>(eRectangleIndex::Size); ++i)
		{
			float x = rectangle.mVertices[i].GetX();
			float y = rectangle.mVertices[i].GetY();

			if (minX > x)
			{
				minX = x;
			}
			else if (maxX < x)
			{
				maxX = x;
			}

			if (minY > y)
			{
				minY = y;
			}
			else if (maxY < y)
			{
				maxY = y;
			}
		}

		return hRectangle(Vector2(minX, minY), Vector2(maxX, maxY));
	}
}