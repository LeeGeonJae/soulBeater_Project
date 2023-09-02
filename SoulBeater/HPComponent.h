#pragma once

#include "IUpdateable.h"
#include "SceneManager.h"
#include "SpriteRenderer.h"
#include "UIManager.h"

namespace soulBeater
{
	class HPComponent :
		public d2dFramework::SpriteRenderer,
		public d2dFramework::IUpdateable
	{
	public:
		HPComponent(unsigned id, d2dFramework::GameObject* owner);
		~HPComponent() override = default;

		void Init() override;
		void Release() override;

		void Update(float deltaTime) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;

		inline void SetHP(unsigned int hp);

		inline d2dFramework::GameObject* GetGameObject() const override;
		inline unsigned GetId() const override;
		inline unsigned int GetHP() const;

	private:
		std::array<D2D_RECT_F, 3> mImageRect;
		std::array<D2D_RECT_F, 11> mNumber;

		unsigned int mHP;
	};

	void HPComponent::SetHP(unsigned int hp)
	{
		mHP = hp;
	}

	unsigned HPComponent::GetId() const
	{
		return SpriteRenderer::GetId();
	}
	d2dFramework::GameObject* HPComponent::GetGameObject() const
	{
		return SpriteRenderer::GetGameObject();
	}
	unsigned int HPComponent::GetHP() const
	{
		return mHP;
	}
}

