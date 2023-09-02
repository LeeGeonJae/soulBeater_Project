#include "HPComponent.h"

#include "EnumButtons.h"
#include "EventManager.h"
#include "RenderManger.h"
#include "Transform.h"


void d2dFramework::HPComponent::Init()
{
	SpriteRenderer::Init();
	IUpdateable::Init();
	EventManager::GetInstance()->RegisterBroadcastEvent("SetUISceneManager", std::to_string(this->GetGameObject()->GetId()));
	SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Number"));
	SetUVRectangle(mNumber[0]);
	SetTransformLayer(eTransformLayer::UI);
	SetSpriteType(eSpriteType::Sprite);
	SetIsActive(false);
	for (size_t i = 0; i < 11; i++)
	{
		float RectWidth = GetBitmap()->GetPixelSize().width / 11;
		float RectHeight = GetBitmap()->GetPixelSize().height;
		mSize = { RectWidth ,RectHeight };
		for (size_t i = 0; i < 11; i++)
		{
			mNumber[i] = { RectWidth * i ,0,RectWidth * (i + 1) - 1,RectHeight };
		}
	}


}

void d2dFramework::HPComponent::Release()
{
	SpriteRenderer::Release();
	IUpdateable::Release();
}

void d2dFramework::HPComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
{
	if (!mbIsActive)
	{
		return;
	}

	Transform* transform = GetGameObject()->GetComponent<Transform>();
	D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(mbIsLeft ? -1 : 1, -1) * transform->GetTransform();
	GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetUITransform());
	D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);
	Vector2 Size = { mNumber[0].right,mNumber[0].bottom };
	GetRenderManager()->DrawBitMap({0,0}, Size * 2, mImageRect[0], GetBitmap());
	GetRenderManager()->DrawBitMap({ Size.GetX()*2,0} , Size*2, mImageRect[1], GetBitmap());
	GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
	GetRenderManager()->SetColor(prevColor);
}

void d2dFramework::HPComponent::Update(float deltaTime)
{
	int hp=30;
	/// 
	if (mUIManager->GetUIManagerType() == soulBeater::eUIManagerType::InGame
		|| mUIManager->GetUIManagerType() == soulBeater::eUIManagerType::InGamePause
		|| mUIManager->GetUIManagerType() == soulBeater::eUIManagerType::SettingOption)
	{
		mbIsActive = true;
	}
	else
	{
		mbIsActive = false;
	}



	if (hp >= 10)
	{
		mImageRect[0] = mNumber[hp / 10];
		mImageRect[1] = mNumber[hp % 10];

	}
	else
	{
		mImageRect[0] = mNumber[0];
		mImageRect[1] = mNumber[hp];

	}
}
