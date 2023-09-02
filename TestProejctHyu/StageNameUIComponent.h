#pragma once
#include "IUpdateable.h"
#include "SpriteRenderer.h"
#include "../TestProejctHyu/UIManager.h"



namespace d2dFramework
{
	class StageNameUIComponent :
		public SpriteRenderer,public IUpdateable
	{
	public:
		StageNameUIComponent(unsigned id, GameObject* owner)
			: SpriteRenderer(id, owner)
		{
		}

		~StageNameUIComponent() override=default;
		void Init() override;
		void Release() override;
		void Update(float deltaTime) override;
		void Render(const D2D1::Matrix3x2F& cameraTransform) override;
		inline void SetUIManager(UIManager* UIManager);
		inline void SetSceneManager(SceneManager* sceneManager);
		inline GameObject* GetGameObject() const override;
		inline unsigned GetId() const override;

	private:
		std::array<D2D_RECT_F, 9> mStageNameRect;
		UIManager* mUIManager;
		SceneManager* mSceneManager;
		Scene* mcurrentScene;
	};

	void StageNameUIComponent::SetUIManager(UIManager* UIManager)
	{
		mUIManager = UIManager;
	}
	void StageNameUIComponent::SetSceneManager(SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}
	GameObject* StageNameUIComponent:: GetGameObject()const
	{
		return SpriteRenderer::GetGameObject();
	}
	unsigned StageNameUIComponent::GetId() const
	{
		return SpriteRenderer::GetId();
	}

}
