#pragma once
#include "IUpdateable.h"
#include "SceneManager.h"
#include "SpriteRenderer.h"
#include "../TestProejctHyu/UIManager.h"


namespace d2dFramework
{
    class HPComponent :
        public SpriteRenderer,public IUpdateable
    {
    public:
		HPComponent(unsigned id, GameObject* owner)
			: SpriteRenderer(id, owner)
		{
		}
	    ~HPComponent() override = default;
	    void Init() override;
	    void Release() override;
	    void Render(const D2D1::Matrix3x2F& cameraTransform) override;
		inline void SetUIManager(UIManager* UIManager);
		inline void SetSceneManager(SceneManager* sceneManager);

		inline GameObject* GetGameObject() const override;
		inline unsigned GetId() const override;
	    void Update(float deltaTime) override;
    private:
		std::array<D2D_RECT_F,2> mImageRect;
		std::array<D2D_RECT_F, 11> mNumber;
		UIManager* mUIManager;
		SceneManager* mSceneManager;
    };

	void HPComponent::SetUIManager(UIManager* UIManager)
	{
		mUIManager = UIManager;
	}
	void HPComponent::SetSceneManager(SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
	}

	unsigned HPComponent:: GetId() const
	{
		return SpriteRenderer::GetId();
	}
	GameObject* HPComponent::GetGameObject() const
	{
		return SpriteRenderer::GetGameObject();
	}

}

