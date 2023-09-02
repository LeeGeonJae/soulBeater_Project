#include "StageNameUIComponent.h"

#include "EventManager.h"
#include "RenderManger.h"
#include "Transform.h"
#include "../SoulBeater/SoulBeaterScene.h"

namespace d2dFramework
{
	void StageNameUIComponent::Init()
	{
		SpriteRenderer::Init();
		IUpdateable::Init();
		EventManager::GetInstance()->RegisterBroadcastEvent("SetUISceneManager", std::to_string(this->GetGameObject()->GetId()));
   		SetBitmap(GetRenderManager()->GetBitmapOrNull(L"StageName"));
		float height=GetBitmap()->GetPixelSize().height;
		float width = GetBitmap()->GetPixelSize().width / 9;
		mSize = { width ,height };
		for (int i=0;i<9;i++)
		{
			
			mStageNameRect[i] = { width* i ,0,width * (i + 1)-1,height };
		}



	}

	void StageNameUIComponent::Release()
	{
		SpriteRenderer::Release();
		IUpdateable::Release();
	}
	void StageNameUIComponent::Update(float deltaTime)
	{
		
	}
	void StageNameUIComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		if (!mbIsActive)
		{
			return;
		}

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(mbIsLeft ? -1 : 1, -1) * transform->GetTransform();

		GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetUITransform());


		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(mBorderColor);


		GetRenderManager()->DrawBitMap(mOffset, mSize, mUVRectangle, mBitmap);

		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->SetColor(prevColor);
	}
}
