#pragma once

namespace d2dFramework
{
	struct GridPosition
	{
		unsigned int X;
		unsigned int Y;

		friend bool operator==(const GridPosition& lhs, const GridPosition& rhs)
		{
			return lhs.X == rhs.X && lhs.Y == rhs.Y;
		}

		friend bool operator!=(const GridPosition& lhs, const GridPosition& rhs)
		{
			return !(lhs == rhs);
		}

		friend bool operator<(const GridPosition& lhs, const GridPosition& rhs)
		{
			if (lhs.X == rhs.X)
			{
				return lhs.Y < rhs.Y;
			}

			return lhs.X < rhs.X;
		}
	};
}