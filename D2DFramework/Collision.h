#pragma once

namespace d2dFramework
{
	struct AABB;
	struct OBB;
	struct Circle;
	struct Manifold;
	class GameObject;

	class Collision
	{
	public:
		// 모든 충돌로 생성된 manifold는 lhs->rhs로 가는 방향으로 생성되므로 lhs는 역방향에 대한 임펄스를 가해야한다.
		static bool CheckAABBToAABB(const AABB& lhs, const AABB& rhs, Manifold* outManifold);
		static bool CheckAABBToOBB(const AABB& lhs, const OBB& rhs);
		static bool CheckAABBToOBB(const AABB& lhs, const OBB& rhs, Manifold* outManifold);
		static bool CheckAABBToCircle(const AABB& lhs, const Circle& rhs, Manifold* outManifold);

		static bool CheckOBBToOBB(const OBB& lhs, const OBB& rhs, Manifold* outManifold);
		static bool CheckOBBToCircle(const OBB& lhs, const Circle& rhs, Manifold* outManifold);

		static bool CheckCircleToAABB(const Circle& lhs, const AABB& rhs, Manifold* outManifold);
		static bool CheckCircleToCircle(const Circle& lhs, const Circle& rhs, Manifold* outManifold);

		static float GetWidth(const AABB& aabb);
		static float GetWidth(const OBB& obb);

		static float GetHeight(const AABB& aabb);
		static float GetHeight(const OBB& obb);

		static Vector2 GetSize(const AABB& aabb);
		static Vector2 GetSize(const OBB& obb);

		static Vector2 GetCenter(const AABB& aabb);
		static Vector2 GetCenter(const OBB& aabb);

		static AABB MakeAABB(const Vector2& offset, const Vector2& size, D2D1::Matrix3x2F transform);
		static AABB MakeAABB(const Vector2& offset, const Vector2& size, const Vector2& scale, const Vector2& translate);
		static OBB MakeOBB(const Vector2& offset, const Vector2& size, D2D1::Matrix3x2F transform);
		static Circle MakeCircle(const Vector2& offset, float radius, D2D1::Matrix3x2F transform);
		static Circle MakeCircle(const Vector2& offset, float radius, const Vector2& scale, const Vector2& translate);
	};
}