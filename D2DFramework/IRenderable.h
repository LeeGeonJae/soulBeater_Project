#pragma once

#include <d2d1.h>

namespace d2dFramework
{
	class RenderManager;
	class GameObject;
	struct AABB;

	class IRenderable
	{
		friend class GameProcessor;

	public:
		IRenderable() = default;
		virtual ~IRenderable() = default;

		virtual void Init() = 0;
		virtual bool IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox) = 0;
		virtual void Render(const D2D1::Matrix3x2F& cameraTransform) = 0;
		virtual void Release() = 0;

		virtual GameObject* GetGameObject() const = 0;
		virtual inline unsigned int GetId() const = 0;

	protected:
		inline RenderManager* GetRenderManager() const;

	private:
		static void SetRenderManager(RenderManager* renderManager);

	private:
		static RenderManager* mRenderManager;
	};

	RenderManager* IRenderable::GetRenderManager() const
	{
		return mRenderManager;
	}
}