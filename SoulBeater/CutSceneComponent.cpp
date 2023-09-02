#include "CutSceneComponent.h"

#include "ObjectManager.h"
#include "RenderManger.h"
#include "InputManager.h"

namespace soulBeater
{
	CutSceneComponent::CutSceneComponent(unsigned int id, d2dFramework::GameObject* owner)
		: d2dFramework::SpriteRenderer(id, owner)
		, d2dFramework::IUpdateable()
		, mbIsDeleteButtonPushed()
		, mElapsed()
	{
	}

	void CutSceneComponent::Init()
	{
		SpriteRenderer::Init();
		IUpdateable::Init();

		mbIsDeleteButtonPushed = false;
		mElapsed = 2.f;
	}
	void CutSceneComponent::Release()
	{
		SpriteRenderer::Release();
		IUpdateable::Release();
	}

	void CutSceneComponent::Update(float deltaTime)
	{
		using namespace d2dFramework;

		if (InputManager::GetInstance()->GetKeyState(VK_SPACE) == eKeyState::Push)
		{
			mbIsDeleteButtonPushed = true;
		}

		if (mbIsDeleteButtonPushed)
		{
			mElapsed -= deltaTime;
		}

		if (mElapsed < 0.f)
		{
			ObjectManager::GetInstance()->DeleteObject(GetGameObject()->GetId());
		}
	}

	void CutSceneComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		using namespace d2dFramework;

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
			GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetGridObjectTransform());
			break;
		case eTransformLayer::UI:			
			GetRenderManager()->SetTransform(GetRenderManager()->GetUITransform() * matrix);
			break;
		default:
			break;
		}
		
		assert(mBitmap != nullptr);
		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->DrawBitMap(mUVRectangle, mUVRectangle, mBitmap, mElapsed * 0.5f);
	}
}