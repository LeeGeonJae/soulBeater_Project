#pragma once

#include "BaseEntity.h"
#include "IncludeManager.h"
#include "IncludeComponent.h"
#include "SoulBeaterScene.h"
#include "eUIManagerType.h"
#include "eButtonType.h"
#include "UIComponent.h"
#include "ButtonComponent.h"
#include "GameObject.h"

namespace d2dFramework
{
	class GameObject;
}

namespace soulBeater
{
	class UIComponent;
	class ButtonComponent;

	class UIManager final : public d2dFramework::BaseEntity
	{
	public:
		UIManager();
		~UIManager() override = default;

		void Init(d2dFramework::SceneManager* sceneManager);
		void Release();
		void Update(float deltaTime);
		//void ResetmSelect();

		soulBeater::eUIManagerType GetUIManagerType() { return mUIManagerType; }
		void SetmButtonManagerType(soulBeater::eUIManagerType type) { mUIManagerType = type; }
		void SetIsActive(bool Active) { mbIsButtonUsable = Active; }
		bool Test = false;
		void SetSceneManager(d2dFramework::SceneManager* sceneManager) { mSceneManager = sceneManager; }
		bool GetIsActive() { return mbIsButtonUsable; }

	private:
		void updateStartScene(float deltaTime);
		void updateHelpsScene(float deltaTime);
		void updateInGameOption(float deltaTime);
		void updateSettingOption(float deltaTime);
		void updateGameOverScene(float deltaTime);
		ButtonComponent* FindExactButtonComponent();

		inline d2dFramework::GameObject* FindObject(unsigned int id);
		inline UIComponent* FindObjectUIComponent(unsigned int id);
		inline ButtonComponent* FindObjectButtonComponent(unsigned int id);
		inline d2dFramework::SpriteRenderer* FindObjectSpriteRenderer(unsigned int id);
		inline soulBeater::SoulBeaterScene* GetCurrentScene();

		void ButtonSetting(d2dFramework::GameObject*);
		void UISetting(d2dFramework::GameObject*);
		void TimerUISetting(d2dFramework::GameObject*);
		soulBeater::eUIManagerType mUIManagerType;
		eButtonType mSelected;
		bool mbIsButtonUsable;
		d2dFramework::SceneManager* mSceneManager;
		d2dFramework::Scene* mCurrentScene;

		std::map<unsigned int, soulBeater::eUIManagerType> mButtonIDs;
		std::map<unsigned int, soulBeater::eUIManagerType > mSpritIDs;
		d2dFramework::GameObject* mTimer;
	};

	soulBeater::SoulBeaterScene* UIManager::GetCurrentScene()
	{
		return static_cast<soulBeater::SoulBeaterScene*>(mSceneManager->GetCurrentScene());
	}

	d2dFramework::GameObject* UIManager::FindObject(unsigned int id)
	{
		return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
	}
	UIComponent* UIManager::FindObjectUIComponent(unsigned int id)
	{
		using namespace d2dFramework;

		GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(id, true);
		object->GetComponent<UIComponent>();
		return object->GetComponent<UIComponent>();
	}
	ButtonComponent* UIManager::FindObjectButtonComponent(unsigned int id)
	{
		return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true)->GetComponent<ButtonComponent>();
	}
	d2dFramework::SpriteRenderer* UIManager::FindObjectSpriteRenderer(unsigned int id)
	{
		return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true)->GetComponent<d2dFramework::SpriteRenderer>();
	}
}
