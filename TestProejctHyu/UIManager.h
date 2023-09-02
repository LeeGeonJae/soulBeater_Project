#pragma once
#include "BaseEntity.h"
#include "ButtonComponent.h"
#include "GameObject.h"
#include "EnumButtons.h"
#include "IncludeManager.h"
#include "IncludeComponent.h"
#include "UIComponent.h"
//#include "SoulBeaterScene.h"
#include "../SoulBeater/SoulBeaterScene.h"
namespace d2dFramework
{
	class UIComponent;

	class UIManager final : public d2dFramework::BaseEntity
	{
	public:
		UIManager();
		~UIManager() override = default;

		void Init(d2dFramework::SceneManager* sceneManager);
		void Release();
		void Update(float deltaTime);
		void ResetmSelect()
		{
			if (mSelected == eButtonType::eButtonTypeEnd)
			{
				switch (mUIManagerType)
				{
				case soulBeater::eUIManagerType::StartScene: {mSelected = eButtonType::StartButton; }break;
				case soulBeater::eUIManagerType::InGamePause: {mSelected = eButtonType::ResumeButton; }break;
				case soulBeater::eUIManagerType::SettingOption: {mSelected = eButtonType::SoundButton; }break;
				case soulBeater::eUIManagerType::InGame: break;
				case soulBeater::eUIManagerType::EndingScene: break;
				case soulBeater::eUIManagerType::None: break;
				default: ;
				}
			}
		}



		soulBeater::eUIManagerType GetUIManagerType() { return mUIManagerType; }
		void SetmButtonManagerType(soulBeater::eUIManagerType type) { mUIManagerType = type; }
		void SetIsActive(bool Active) {mbIsButtonUsable= Active; }
		bool Test = false;
		void SetSceneManager(d2dFramework::SceneManager* sceneManager) { mSceneManager = sceneManager; }
		bool GetIsActive() {return mbIsButtonUsable;}

	private:
		void updateStartScene(float deltaTime);
		//void updateContentsScene(float deltaTime);
		void updateInGameOption(float deltaTime);
		void updateSettingOption(float deltaTime);
		d2dFramework::ButtonComponent* FindExactButtonComponent();
		inline d2dFramework::GameObject* FindObject(unsigned int id)
		{
			return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
		}
		inline UIComponent* FindObjectUIComponent(unsigned int id)
		{
			return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true)->GetComponent<d2dFramework::UIComponent>();
		}
		inline d2dFramework::ButtonComponent* FindObjectButtonComponent(unsigned int id)
		{
			return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true)->GetComponent<d2dFramework::ButtonComponent>();
		}
		inline d2dFramework::SpriteRenderer* FindObjectSpriteRenderer(unsigned int id)
		{
			return d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true)->GetComponent<d2dFramework::SpriteRenderer>();
		}
		inline soulBeater::SoulBeaterScene* GetCurrentScene();

		void ButtonSetting(GameObject*);
		void UISetting(GameObject*);
		void TimerUISetting(GameObject*);
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
}
