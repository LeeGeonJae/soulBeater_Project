#pragma once

#include <d2d1.h>

#include "Component.h"
#include "IRenderable.h"
#include "IUpdateable.h"
#include "Vector2.h"

namespace d2dFramework
{
	class GameObject;
	class AnimationAsset;

	class AnimationRenderer final : public Component, public IRenderable, public IUpdateable
	{
	public:
		AnimationRenderer(unsigned int id, GameObject* owner);
		~AnimationRenderer() override = default;

		void Init() override;
		void Release() override;

		void Update(float deltaTime) override;
		bool IsOutsideBoundingBox(const D2D1::Matrix3x2F& cameraTransform, const AABB& boundingBox) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;

		inline void SetOffSet(const Vector2& offset);
		inline void SetSize(const Vector2& size);
		inline void SetAnimationAsset(AnimationAsset* animationAsset);

		inline GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline const Vector2& GetOffset(void) const;
		inline const Vector2& GetSize(void) const;
		inline AnimationAsset* GetAnimationAsset(void) const;

	private:
		Vector2 mOffset;
		Vector2 mSize;

		unsigned int mAnimationIndex;
		unsigned int mFrameIndex;
		float mAccumulatedTime;
		bool mbIsLoop;

		AnimationAsset* mAnimationAsset;
	};

	void AnimationRenderer::SetOffSet(const Vector2& offset)
	{
		mOffset = offset;
	}
	void AnimationRenderer::SetSize(const Vector2& size)
	{
		mSize = size;
	}
	void AnimationRenderer::SetAnimationAsset(AnimationAsset* animationAsset)
	{
		mAnimationAsset = animationAsset;
	}

	GameObject* AnimationRenderer::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int AnimationRenderer::GetId() const
	{
		return BaseEntity::GetId();
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
}