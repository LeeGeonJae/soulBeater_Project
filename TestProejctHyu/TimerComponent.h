#pragma once
#include "SpriteRenderer.h"
#include "../TestProejctHyu/UIManager.h"


namespace d2dFramework
{

	class TimerComponent :
		public SpriteRenderer,public IUpdateable
	{
	public:
		TimerComponent(unsigned id, GameObject* owner)
			: SpriteRenderer(id, owner)
		{
		}

		~TimerComponent() override = default;
		void Init() override;
		void Release() override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;
		inline GameObject* GetGameObject() const override;
		inline unsigned GetId() const override;
		void SetUIManager(UIManager* UIManager) { mUIManager = UIManager; }
		void SetSceneManager(SceneManager* sceneManager) { mSceneManager = sceneManager; }
		void Update(float deltaTime) override;
		

	private:
		std::array<D2D_RECT_F, 4> mNumberRect;
	std::array<D2D_RECT_F, 11> mNumber;
		UIManager* mUIManager;
		SceneManager* mSceneManager;
		Scene* mCurrentScene;
		float mMaxtime;
		float mCurrenttime;



	};
	GameObject* TimerComponent::GetGameObject() const
	{
		return SpriteRenderer::GetGameObject();
	}

	unsigned TimerComponent::GetId() const
	{
		return SpriteRenderer::GetId();
	}

}
