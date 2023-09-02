#pragma once

#include "IUpdateable.h"
#include "SpriteRenderer.h"

namespace d2dFramework
{
	class GameObject;
}

namespace soulBeater
{
	class CutSceneComponent final :
		public d2dFramework::IUpdateable,
		public d2dFramework::SpriteRenderer
	{
	public:
		CutSceneComponent(unsigned int id, d2dFramework::GameObject* owner);
		~CutSceneComponent() = default;

		void Init() override;
		void Release() override;

		void Update(float deltaTime) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;

		inline void SetElasped(float elpased);

		inline d2dFramework::GameObject* GetGameObject() const override;
		inline unsigned int GetId() const override;
		inline float GetElapsed() const;

	private:
		bool mbIsDeleteButtonPushed;
		float mElapsed;
	};

	void CutSceneComponent::SetElasped(float elpased)
	{
		mElapsed = elpased;
	}

	d2dFramework::GameObject* CutSceneComponent::GetGameObject() const
	{
		return Component::GetGameObject();
	}
	unsigned int CutSceneComponent::GetId() const
	{
		return BaseEntity::GetId();
	}
	float CutSceneComponent::GetElapsed() const
	{
		return mElapsed;
	}
}