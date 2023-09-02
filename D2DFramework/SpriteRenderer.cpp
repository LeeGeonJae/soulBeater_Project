#include "SpriteRenderer.h"

#include "GameObject.h"
#include "Transform.h"
#include "RenderManger.h"
#include "AABB.h"
#include "OBB.h"
#include "Collision.h"
#include "GridComponent.h"

#include <cassert>
#include <locale>
#include <codecvt>

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
		, mbIsActive(true)
		, mbIsLeft(false)
		, mTransformLayer(eTransformLayer::None)
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
		if (!mbIsActive)
		{
			return;
		}

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(mbIsLeft ? -1.f : 1.f, -1.f) * transform->GetTransform();

		switch (mTransformLayer)
		{
		case eTransformLayer::None:
			GetRenderManager()->SetTransform(matrix * cameraTransform);
			break;
		case eTransformLayer::Grid:
		{
			D2D1::Matrix3x2F tempMatrix = GetRenderManager()->GetGridObjectTransform();
			GetRenderManager()->SetTransform(matrix * tempMatrix);
			break;
		}
		case eTransformLayer::UI:
			GetRenderManager()->SetTransform(GetRenderManager()->GetUITransform() * matrix);
			break;
		default:
			break;
		}

		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);

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
		case d2dFramework::eSpriteType::Inheritance:
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
		mbIsActive = false;
	}

	void SpriteRenderer::SerializeIn(nlohmann::ordered_json& object)
	{
		mBitmapKey = object["mBitmapKey"];
		mOffset.SetXY(object["mOffset"][0], object["mOffset"][1]);
		mSize.SetXY(object["mSize"][0], object["mSize"][1]);
		mBaseColor = { object["mBaseColor"][0]
			,object["mBaseColor"][1]
			,object["mBaseColor"][2]
			,object["mBaseColor"][3] };
		mBorderColor = { object["mBorderColor"][0]
			,object["mBorderColor"][1]
			,object["mBorderColor"][2]
			,object["mBorderColor"][3] };
		mUVRectangle = { object["mUVRectangle"][0]
			,object["mUVRectangle"][1]
			,object["mUVRectangle"][2]
			,object["mUVRectangle"][3] };
		mSpriteType = (eSpriteType)object["mSpriteType"];
		///mBitmap 얘는 어떻게 serializeout 할까??
		///비트맵 Path를 Json에 저장해서 Bitmap 설정하는 방식이 좋지 않을까?
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		std::wstring wstr = converter.from_bytes(mBitmapKey);

		mBitmap = GetRenderManager()->GetBitmapOrNull(wstr.c_str());
		///TODO 일단 여기 BtiMap전용 자료형 Const WCHAR*로 만들었습니다.
		const WCHAR* wideString = reinterpret_cast<const WCHAR*>(mBitmapKey.c_str());

	}

	void SpriteRenderer::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "SpriteRenderer";
		Component::SerializeOut(object);
		object["mBitmapKey"] = mBitmapKey;
		object["mOffset"] = { mOffset.GetX(),mOffset.GetY() };
		object["mSize"] = { mSize.GetX(),mSize.GetY() };
		object["mBaseColor"] = { mBaseColor.r,mBaseColor.g,mBaseColor.b,mBaseColor.a };
		object["mBorderColor"] = { mBorderColor.r,mBorderColor.g,mBorderColor.b,mBorderColor.a };
		object["mUVRectangle"] = { mUVRectangle.left,mUVRectangle.top,mUVRectangle.right,mUVRectangle.bottom };
		object["mSpriteType"] = mSpriteType;

		///TODO 여기도 BitmapPath 저장해놨는데, 연결방법을 아직 구현안함.
	}
};
