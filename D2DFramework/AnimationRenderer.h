#pragma once

#include <cassert>

#include "Component.h"
#include "IRenderable.h"
#include "IUpdateable.h"
#include "Vector2.h"
#include "AnimationAsset.h"
#include "eTransformLayer.h"

#include <d2d1.h>
#include <d2d1_1.h>

namespace d2dFramework
{
	class GameObject;

	class AnimationRenderer final : public Component, public IRenderable, public IUpdateable
	{
	public:
		AnimationRenderer(unsigned int id, GameObject* owner);
		~AnimationRenderer() override = default;

		void Init() override;
		void Release() override;
		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;
		void Update(float deltaTime) override;
		bool IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;

		inline void SetIsLeft(bool bIsLeft);
		inline void SetIsLoop(bool bIsLoop);
		inline void SetOffSet(const Vector2& offset);
		inline void SetSize(const Vector2& size);
		inline void SetAnimationIndex(unsigned int index);
		inline void SetFrameindex(unsigned int index);
		inline void SetAnimationAsset(AnimationAsset* animationAsset);
		inline void SetIsActive(bool bIsActive);
		inline void SetTransformLayer(eTransformLayer transformLayer);

		inline bool GetIsLeft();
		inline bool GetIsLoop() const;
		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;

		inline bool GetIsAnimationEnd() const;
		inline unsigned int GetAnimationIndex() const;
		inline unsigned int GetFrameindex() const;
		inline const Vector2& GetOffset(void) const;
		inline const Vector2& GetSize(void) const;
		inline AnimationAsset* GetAnimationAsset(void) const;
		inline bool GetIsActive() const;
		inline eTransformLayer GetTransformLayer() const;

	private:
		Vector2 mOffset;
		Vector2 mSize;

		unsigned int mAnimationIndex;
		unsigned int mFrameIndex;
		float mAccumulatedTime;
		bool mbIsLoop;

		AnimationAsset* mAnimationAsset;
		std::string mAnimationKey;

		bool mbIsActive;
		bool mbIsLeft;

		eTransformLayer mTransformLayer;
	};

	void AnimationRenderer::SetIsLeft(bool bIsLeft)
	{
		mbIsLeft = bIsLeft;
	}

	void AnimationRenderer::SetIsLoop(bool bIsLoop)
	{
		mbIsLoop = bIsLoop;
	}
	void AnimationRenderer::SetOffSet(const Vector2& offset)
	{
		mOffset = offset;
	}
	void AnimationRenderer::SetSize(const Vector2& size)
	{
		mSize = size;
	}

	void AnimationRenderer::SetAnimationIndex(unsigned int animationIndex)
	{
		assert(animationIndex < mAnimationAsset->GetAnimationSize());
		mAnimationIndex = animationIndex;
	}
	void AnimationRenderer::SetFrameindex(unsigned int frameIndex)
	{
		// assert(frameIndex != 0 && frameIndex < mAnimationAsset->GetFrameSize(mAnimationIndex));
		mFrameIndex = frameIndex;
	}
	void AnimationRenderer::SetAnimationAsset(AnimationAsset* animationAsset)
	{
		assert(animationAsset != nullptr);
		mAnimationAsset = animationAsset;
	}
	void AnimationRenderer::SetIsActive(bool bIsActive)
	{
		mbIsActive = bIsActive;
	}
	void AnimationRenderer::SetTransformLayer(eTransformLayer transformLayer)
	{
		mTransformLayer = transformLayer;
	}

	bool AnimationRenderer::GetIsLeft()
	{
		return mbIsLeft;
	}

	bool AnimationRenderer::GetIsLoop() const
	{
		return mbIsLoop;
	}
	GameObject* AnimationRenderer::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int AnimationRenderer::GetId() const
	{
		return BaseEntity::GetId();
	}
	bool AnimationRenderer::GetIsAnimationEnd() const
	{
		return mFrameIndex == mAnimationAsset->GetFrameSize(mAnimationIndex) - 1;
	}
	unsigned int AnimationRenderer::GetAnimationIndex() const
	{
		return mAnimationIndex;
	}
	unsigned int AnimationRenderer::GetFrameindex() const
	{
		return mFrameIndex;
	}
	const Vector2& AnimationRenderer::GetOffset() const
	{
		return mOffset;
	}
	const Vector2& AnimationRenderer::GetSize() const
	{
		return mSize;
	}
	AnimationAsset* AnimationRenderer::GetAnimationAsset(void) const
	{
		return mAnimationAsset;
	}
	bool AnimationRenderer::GetIsActive() const
	{
		return mbIsActive;
	}
	eTransformLayer AnimationRenderer::GetTransformLayer() const
	{
		return mTransformLayer;
	}
}