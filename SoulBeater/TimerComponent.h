#pragma once
#include "SpriteRenderer.h"
#include "UIManager.h"

namespace soulBeater
{
	class TimerComponent :
		public d2dFramework::SpriteRenderer,
		public d2dFramework::IUpdateable
	{
	public:
		TimerComponent(unsigned id, d2dFramework::GameObject* owner);
		~TimerComponent() override = default;

		void Init() override;
		void Release() override;

		void Update(float deltaTime) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;

		inline void SetCurrentTime(float time);

		inline d2dFramework::GameObject* GetGameObject() const override;
		inline unsigned GetId() const override;
		inline float GetCurrentTime() const;

	private:
		std::array<D2D_RECT_F, 4> mNumberRect;
		std::array<D2D_RECT_F, 11> mNumber;
		float mCurrentTime;

		bool mbActive;
	};

	void TimerComponent::SetCurrentTime(float time)
	{
		mCurrentTime = time;
	}

	d2dFramework::GameObject* TimerComponent::GetGameObject() const
	{
		return SpriteRenderer::GetGameObject();
	}
	unsigned TimerComponent::GetId() const
	{
		return SpriteRenderer::GetId();
	}
	float TimerComponent::GetCurrentTime() const
	{
		return mCurrentTime;
	}
}
