#include "AnimationRenderer.h"

#include "AnimationAsset.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManger.h"
#include "EventManager.h"
#include "Collision.h"
#include "OBB.h"

#include <cassert>

namespace d2dFramework
{
	AnimationRenderer::AnimationRenderer(unsigned int id, GameObject* owner)
		: Component(id, owner)
		, mOffset(0, 0)
		, mSize(10, 10)
		, mbIsLoop(false)
		, mAccumulatedTime(0.f)
		, mFrameIndex(0)
		, mAnimationIndex(0)
		, mAnimationAsset(nullptr)
	{
	}

	void AnimationRenderer::Init()
	{
		IRenderable::Init();
		IUpdateable::Init();
	}

	void AnimationRenderer::Update(float deltaTime)
	{
		assert(mAnimationAsset != nullptr);
		mAccumulatedTime += deltaTime;

		FrameInformation current = mAnimationAsset->GetFrameInformation(mAnimationIndex, mFrameIndex);
		while (mAccumulatedTime >= current.AnimationTime)
		{
			++mFrameIndex;

			if (mFrameIndex == mAnimationAsset->GetFrameSize(mAnimationIndex))
			{
				if (mbIsLoop)
				{
					mFrameIndex = 0;
				}
				else
				{
					--mFrameIndex;
				}
			}

			mAccumulatedTime -= current.AnimationTime;
			current = mAnimationAsset->GetFrameInformation(mAnimationIndex, mFrameIndex);
		}
	}

	bool AnimationRenderer::IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox)
	{
		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F combineTransform = transform->GetTransform() * cameraTransform;

		OBB rendererOBB = Collision::MakeOBB(mSize, mOffset, combineTransform);

		return !Collision::CheckAABBToOBB(boundingBox, rendererOBB);
	}

	void AnimationRenderer::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		assert(mAnimationAsset != nullptr);

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = transform->GetTransform();
		GetRenderManager()->SetTransform(matrix * cameraTransform);
		{
			const FrameInformation& frameInfo = mAnimationAsset->GetFrameInformation(mAnimationIndex, mFrameIndex);
			GetRenderManager()->DrawBitMap(mOffset, mSize, frameInfo.UVRectangle, mAnimationAsset->GetBitmap());
		}
		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
	}

	void AnimationRenderer::Release()
	{
		IRenderable::Release();
		IUpdateable::Release();
	}

	void AnimationRenderer::SerializeIn(nlohmann::ordered_json& object)
	{
		mOffset.SetXY(object["mOffset"][0], object["mOffset"][1]);
		mSize.SetXY(object["mSize"][0], object["mSize"][1]);
		mAnimationKey = object["mAnimationKey"];
		///TODO 지환이형 도와주세여 여기에 키값으로 애니메이션 에셋 연결하는 거 만들어주세요.
		//AnimationAsset* mAnimationAsset;
	}

	void AnimationRenderer::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "AnimationRenderer";
		Component::SerializeOut(object);
		object["AnimationRenderer_mOffset"] = { mSize.GetX(),mSize.GetY() };
		object["AnimationRenderer_mSize"] = { mSize.GetX(),mSize.GetY() };
		object["mAnimationKey"] = mAnimationKey;
	}
}
