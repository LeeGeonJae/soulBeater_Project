#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "RenderManger.h"
#include "AABB.h"
#include "OBB.h"
#include "Collision.h"

#include <cassert>

namespace d2dFramework
{
	SpriteRenderer::SpriteRenderer(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mOffset(0.f, 0.f)
		, mSize(1.f, 1.f)
		, mBaseColor{ 1.f, 1.f, 1.f, 1.f }
		, mBorderColor{ 0.f, 0.f, 0.f, 1.f }
		, mBitmap(nullptr)
		, mUVRectangle{ 0.f, 0.f, 1.f, 1.f }
		, mSpriteType(eSpriteType::Rectangle)
	{
	}

	void SpriteRenderer::Init()
	{
		IRenderable::Init();
	}

	bool SpriteRenderer::IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox)
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F combineTransform = transform->GetTransform() * cameraTransform;

		OBB rendererOBB = Collision::MakeOBB(mSize, mOffset, combineTransform);

		return !Collision::CheckAABBToOBB(boundingBox, rendererOBB);
	}

	void SpriteRenderer::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = transform->GetTransform();

		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);
		GetRenderManager()->SetTransform(matrix * cameraTransform);
		switch (mSpriteType)
		{
		case d2dFramework::eSpriteType::Rectangle:
			GetRenderManager()->DrawRectangle(mOffset, mSize);
			GetRenderManager()->SetColor(mBaseColor);
			GetRenderManager()->FillRectangle(mOffset, mSize);
			break;
		case d2dFramework::eSpriteType::Circle:
			GetRenderManager()->DrawCircle(mOffset, mSize);
			GetRenderManager()->SetColor(mBaseColor);
			GetRenderManager()->FillCircle(mOffset, mSize);
			break;
		case d2dFramework::eSpriteType::Sprite:
			assert(mBitmap != nullptr);
			GetRenderManager()->DrawBitMap(mOffset, mSize, mUVRectangle, mBitmap);
			break;
		default:
			assert(false);
			break;
		}
		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->SetColor(prevColor);
	}

	void SpriteRenderer::Release()
	{
		IRenderable::Release();
	}
};
