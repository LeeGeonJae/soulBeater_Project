#include "HPComponent.h"

#include "EventManager.h"
#include "RenderManger.h"
#include "Transform.h"
#include "InputManager.h"

namespace soulBeater
{
	HPComponent::HPComponent(unsigned id, d2dFramework::GameObject* owner)
		: SpriteRenderer(id, owner)
		, mHP()
		, mImageRect()
		, mNumber()
	{
	}

	void HPComponent::Init()
	{
		using namespace d2dFramework;

		SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Number"));
		SetUVRectangle(mNumber[0]);
		SetTransformLayer(eTransformLayer::UI);
		SetSpriteType(eSpriteType::Sprite);
		SetIsActive(false);
		for (size_t i = 0; i < 11; i++)
		{
			float RectWidth = static_cast<float>(GetBitmap()->GetPixelSize().width / 11);
			float RectHeight = static_cast<float>(GetBitmap()->GetPixelSize().height);
			mSize = { RectWidth ,RectHeight };
			for (size_t i = 0; i < 11; i++)
			{
				mNumber[i] = { RectWidth * i ,0,RectWidth * (i + 1) - 1,RectHeight };
			}
		}


		SpriteRenderer::Init();
		IUpdateable::Init();
	}

	void HPComponent::Release()
	{
		SpriteRenderer::Release();
		IUpdateable::Release();
	}

	void HPComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		using namespace d2dFramework;

		if (!mbIsActive)
		{
			return;
		}

		unsigned int hundredsPlaceIndex = mHP / 100;
		unsigned int tensPlaceIndex = mHP / 10;
		unsigned int unitPlaceIndex = mHP % 10;

		mImageRect[0] = mNumber[hundredsPlaceIndex % 10];
		mImageRect[1] = mNumber[tensPlaceIndex % 10];
		mImageRect[2] = mNumber[unitPlaceIndex];

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(mbIsLeft ? -1.f : 1.f, -1.f) * transform->GetTransform();
		GetRenderManager()->SetTransform(GetRenderManager()->GetUITransform() * matrix);
		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);
		Vector2 Size = { mNumber[0].right,mNumber[0].bottom };
		GetRenderManager()->DrawBitMap({ 0,0 }, Size * 2, mImageRect[0], GetBitmap());
		GetRenderManager()->DrawBitMap({ Size.GetX() * 2,0 }, Size * 2, mImageRect[1], GetBitmap());
		GetRenderManager()->DrawBitMap({ Size.GetX() * 4,0 }, Size * 2, mImageRect[2], GetBitmap());
		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->SetColor(prevColor);
	}

	void HPComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		if (!mbIsActive)
		{
			return;
		}
	}
}