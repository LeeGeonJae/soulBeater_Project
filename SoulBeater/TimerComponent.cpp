#include "TimerComponent.h"

#include "EventManager.h"
#include "RenderManger.h"
#include "SoundManager.h"
#include "Transform.h"

namespace soulBeater
{
	TimerComponent::TimerComponent(unsigned id, d2dFramework::GameObject* owner)
		: SpriteRenderer(id, owner)
		, mbActive(false)
	{
	}

	void TimerComponent::Init()
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

			for (size_t i = 0; i < 11; i++)
			{
				mNumber[i] = { RectWidth * i ,0,RectWidth * (i + 1) - 1,RectHeight };
			}
			mNumberRect[1] = mNumber[10];
		}

		SpriteRenderer::Init();
		IUpdateable::Init();
	}

	void TimerComponent::Release()
	{
		IUpdateable::Release();
		SpriteRenderer::Release();
	}

	void TimerComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		using namespace d2dFramework;

		if (!mbIsActive)
		{
			return;
		}

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(mbIsLeft ? -1 : 1, -1) * transform->GetTransform();

		switch (mTransformLayer)
		{
		case eTransformLayer::None:
			GetRenderManager()->SetTransform(matrix * cameraTransform);
			break;
		case eTransformLayer::Grid:
			GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetGridObjectTransform());
			break;
		case eTransformLayer::UI:
			GetRenderManager()->SetTransform(GetRenderManager()->GetUITransform() * matrix);
			break;
		default:
			break;
		}
		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);

		assert(mBitmap != nullptr);
		Vector2 Size = { mNumberRect[0].right,mNumberRect[0].bottom };
		Vector2 Drawsize = Size * 1.5;
		float offSetInt = -Drawsize.GetX();
		GetRenderManager()->DrawBitMap({ offSetInt,0 }, Drawsize, mNumberRect[0], GetBitmap());
		GetRenderManager()->DrawBitMap({ offSetInt + Drawsize.GetX(),0 }, Drawsize, mNumberRect[1], GetBitmap());
		GetRenderManager()->DrawBitMap({ offSetInt + Drawsize.GetX() * 2 ,0 }, Drawsize, mNumberRect[2], GetBitmap());
		GetRenderManager()->DrawBitMap({ offSetInt + Drawsize.GetX() * 3 + 6 ,0 }, Drawsize, mNumberRect[3], GetBitmap());
		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->SetColor(prevColor);
	}

	void TimerComponent::Update(float deltaTime)
	{
		if (!mbIsActive)
		{
			return;
		}

		mCurrentTime -= deltaTime;

		if (mCurrentTime <= 0.f)
		{
			mCurrentTime = 0.f;
		}

		int Min = mCurrentTime / 60.f;
		int Second = (int)mCurrentTime % 60;

		if (Second >= 10)
		{
			mNumberRect[2] = mNumber[Second / 10];
			mNumberRect[3] = mNumber[Second % 10];

		}
		else
		{
			mNumberRect[2] = mNumber[0];
			mNumberRect[3] = mNumber[Second];
		}

		mNumberRect[0] = mNumber[Min];
	}
}

