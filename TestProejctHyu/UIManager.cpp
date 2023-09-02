#include "UIManager.h"
#include "eFrameworkID.h"
#include "EventManager.h"
#include "InputManager.h"
#include "MathHelper.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "UIComponent.h"
#include "TimerComponent.h"
#include "HPComponent.h"
//#include "SoulBeaterScene.h"
#include "StageNameUIComponent.h"
#include "../SoulBeater/SoulBeaterScene.h"



namespace d2dFramework
{
	UIManager::UIManager()
		: BaseEntity(static_cast<unsigned int>(d2dFramework::eFrameworkID::UIManager))
		, mUIManagerType(soulBeater::eUIManagerType::StartScene)
		, mbIsButtonUsable(true)
		, mSelected(eButtonType::eButtonTypeEnd)
	{
	}


	void UIManager::Init(d2dFramework::SceneManager* sceneManager)
	{
		mSceneManager = sceneManager;
		auto registerButton = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			d2dFramework::GameObject* button = d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
			if (button != nullptr)
			{
				assert(button->GetComponent<d2dFramework::Transform>() != nullptr);
				assert(button->GetComponent<d2dFramework::ButtonComponent>() != nullptr);
				assert(button->GetComponent<d2dFramework::SpriteRenderer>() != nullptr);
				mButtonIDs.insert({ id, button->GetComponent<d2dFramework::ButtonComponent>()->GetOwnManagerType() });
				button->GetComponent<d2dFramework::ButtonComponent>()->SetUIManager(this);
			}
		};
		auto registerUI = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			d2dFramework::GameObject* UISprite = d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
			if (UISprite != nullptr)
			{
				assert(UISprite->GetComponent<d2dFramework::Transform>() != nullptr);
				assert(UISprite->GetComponent<d2dFramework::SpriteRenderer>() != nullptr);
				assert(UISprite->GetComponent<d2dFramework ::UIComponent>() != nullptr);
				mSpritIDs.insert({ id, UISprite->GetComponent<d2dFramework::UIComponent>()->GetOwnManagerType() });
				UISprite->GetComponent<d2dFramework::UIComponent>()->SetUIManager(this);
			}
		};
		auto SetUISceneManager = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			d2dFramework::GameObject* object = d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
			if (object != nullptr)
			{
				assert(object->GetComponent<d2dFramework::Transform>() != nullptr);
				if (object->GetComponent<d2dFramework::TimerComponent>() != nullptr)
				{
					object->GetComponent<d2dFramework::TimerComponent>()->SetUIManager(this);
					object->GetComponent<d2dFramework::TimerComponent>()->SetSceneManager(mSceneManager);
					assert(object->GetComponent < d2dFramework::HPComponent>() == nullptr);
					assert(object->GetComponent<d2dFramework::StageNameUIComponent>() == nullptr);

				}
				else if (object->GetComponent <d2dFramework::HPComponent>() != nullptr)
				{
					object->GetComponent<d2dFramework::HPComponent>()->SetUIManager(this);
					object->GetComponent<d2dFramework::HPComponent>()->SetSceneManager(mSceneManager);
					assert(object->GetComponent<d2dFramework::TimerComponent>() == nullptr);
					assert(object->GetComponent<d2dFramework::StageNameUIComponent>() == nullptr);
				}
				else if (object->GetComponent <d2dFramework::StageNameUIComponent>() != nullptr)
				{
					object->GetComponent<d2dFramework::StageNameUIComponent>()->SetUIManager(this);
					object->GetComponent<d2dFramework::StageNameUIComponent>()->SetSceneManager(mSceneManager);
					assert(object->GetComponent<d2dFramework::TimerComponent>() == nullptr);
					assert(object->GetComponent < d2dFramework::HPComponent>() == nullptr);

				}

			}
		};
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("RegisterButtonToManager", GetId(), registerButton);
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("RegisterUISpriteToManager", GetId(), registerUI);
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("SetUISceneManager", GetId(), SetUISceneManager);

#pragma region UI생성


		float RectWidth = 1920 / 2;
		float RectHight = 1080 / 2;
		RenderManager::GetInstance()->SetUITransform(D2D1::Matrix3x2F::Identity());
#pragma region Button create
		GameObject* buttonStart = ObjectManager::GetInstance()->CreateObject(501, true);
		GameObject* buttonHelp = ObjectManager::GetInstance()->CreateObject(502, true);
		GameObject* buttonExit = ObjectManager::GetInstance()->CreateObject(503, true);
		GameObject* buttonResume = ObjectManager::GetInstance()->CreateObject(504, true);
		GameObject* buttonSetting = ObjectManager::GetInstance()->CreateObject(505, true);
		GameObject* buttonMenu = ObjectManager::GetInstance()->CreateObject(506, true);
		GameObject* buttonSound = ObjectManager::GetInstance()->CreateObject(507, true);
		GameObject* buttonMusic = ObjectManager::GetInstance()->CreateObject(508, true);
#pragma endregion

#pragma region UI create
		GameObject* UIStart = ObjectManager::GetInstance()->CreateObject(516, true);
		GameObject* UIIngame = ObjectManager::GetInstance()->CreateObject(509, true);
		GameObject* HPRender = ObjectManager::GetInstance()->CreateObject(515, true);
		GameObject* UITimer = ObjectManager::GetInstance()->CreateObject(514, true);
		GameObject* UIIngameOption = ObjectManager::GetInstance()->CreateObject(510, true);
		GameObject* UISettiongOption = ObjectManager::GetInstance()->CreateObject(511, true);
		GameObject* UISoundVolume = ObjectManager::GetInstance()->CreateObject(512, true);
		GameObject* UIMusicVolume = ObjectManager::GetInstance()->CreateObject(513, true);
		GameObject* UIStageNumber = ObjectManager::GetInstance()->CreateObject(517, true);
#pragma endregion

#pragma region Button Setting
		ButtonSetting(buttonStart);
		buttonStart->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::StartButton);
		buttonStart->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::StartScene);
		buttonStart->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"StartButton"));

		ButtonSetting(buttonHelp);
		buttonHelp->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::HelpButton);
		buttonHelp->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::StartScene);
		buttonHelp->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"HelpButton"));

		ButtonSetting(buttonExit);
		buttonExit->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::ExitButton);
		buttonExit->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::StartScene);
		buttonExit->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"ExitButton"));

		ButtonSetting(buttonResume);
		buttonResume->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::ResumeButton);
		buttonResume->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::InGamePause);
		buttonResume->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"ResumeButton"));

		ButtonSetting(buttonSetting);
		buttonSetting->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::SettingButton);
		buttonSetting->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::InGamePause);
		buttonSetting->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"SettingButton"));

		ButtonSetting(buttonMenu);
		buttonMenu->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::MenuButton);
		buttonMenu->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::InGamePause);
		buttonMenu->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"MenuButton"));

		ButtonSetting(buttonSound);
		buttonSound->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::SoundButton);
		buttonSound->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::SettingOption);
		buttonSound->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"SoundButton"));

		ButtonSetting(buttonMusic);
		buttonMusic->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::MusicButton);
		buttonMusic->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::SettingOption);
		buttonMusic->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"MusicButton"));

		///버튼들 위치 잡기
		buttonStart->GetComponent<Transform>()->SetTranslate({ 538,785 });
		buttonHelp->GetComponent<Transform>()->SetTranslate({ 538, 871 });
		buttonExit->GetComponent<Transform>()->SetTranslate({ 538, 948 });
		buttonResume->GetComponent<Transform>()->SetTranslate({ 960, 453 });
		buttonSetting->GetComponent<Transform>()->SetTranslate({ 960, 547 });
		buttonMenu->GetComponent<Transform>()->SetTranslate({ 962, 641 });
		buttonSound->GetComponent<Transform>()->SetTranslate({ 865, 490 });
		buttonMusic->GetComponent<Transform>()->SetTranslate({ 896,589 });

#pragma endregion
#pragma region UI Setting
		UISetting(UIStart);
		UIStart->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::StartScene);
		UIStart->GetComponent<UIComponent>()->SetUIType(eUISpriteType::StartUI);
		UIStart->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"StartUI"));
		UIStart->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });
		UISetting(UIIngame);
		UIIngame->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::InGame);
		UIIngame->GetComponent<UIComponent>()->SetUIType(eUISpriteType::InGameUI);
		UIIngame->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"InGameUI"));
		UIIngame->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });

		UISetting(UIIngameOption);
		UIIngameOption->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::InGamePause);
		UIIngameOption->GetComponent<UIComponent>()->SetUIType(eUISpriteType::InGamePauseUI);
		UIIngameOption->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"InGamePauseUI"));
		UIIngameOption->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });
		UISetting(UISettiongOption);
		UISettiongOption->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::SettingOption);
		UISettiongOption->GetComponent<UIComponent>()->SetUIType(eUISpriteType::SettingUI);
		UISettiongOption->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"SettingUI"));
		UISettiongOption->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });

		UISetting(UISoundVolume);
		UISoundVolume->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::SettingOption);
		UISoundVolume->GetComponent<UIComponent>()->SetUIType(eUISpriteType::SoundMusicUI);
		UISoundVolume->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Volume"));
		UISoundVolume->GetComponent<Transform>()->SetTranslate({ 1127,490 });

		UISetting(UIMusicVolume);
		UIMusicVolume->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::SettingOption);
		UIMusicVolume->GetComponent<UIComponent>()->SetUIType(eUISpriteType::SoundMusicUI);
		UIMusicVolume->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"Volume"));
		UIMusicVolume->GetComponent<Transform>()->SetTranslate({ 1103,581 });

		TimerUISetting(UITimer);
		UITimer->GetComponent<Transform>()->SetTranslate({ 270,90 });
		HPRender->CreateComponent<Transform>(5151);
		HPRender->CreateComponent<HPComponent>(5152);
		HPRender->SetObjectType(eObjectType::UI);
		HPRender->GetComponent<HPComponent>()->SetTransformLayer(eTransformLayer::UI);
		HPRender->GetComponent<Transform>()->SetScale({ 1,-1 });
		HPRender->GetComponent<Transform>()->SetTranslate({ 287,246 });

		UIStageNumber->CreateComponent<Transform>(5171);
		UIStageNumber->CreateComponent<StageNameUIComponent>(5172);
		UIStageNumber->SetObjectType(eObjectType::UI);
		UIStageNumber->GetComponent<StageNameUIComponent>()->SetTransformLayer(eTransformLayer::UI);
		UIStageNumber->GetComponent<Transform>()->SetScale({ 1,-1 });
		UIStageNumber->GetComponent<Transform>()->SetTranslate({ 1727,887 });

#pragma endregion
		///스타트 씬으로 바로 이동!
		//EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", "701");

	}

	void UIManager::Release()
	{
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("RegisterButtonToManager", GetId());
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("RegisterUISpriteToManager", GetId());
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("SetUISceneManager", GetId());
	}

	void UIManager::Update(float deltaTime)
	{
		if (mCurrentScene != nullptr)
		{
			if (mCurrentScene != mSceneManager->GetCurrentScene())
			{
				if (static_cast<soulBeater::SoulBeaterScene*>(mSceneManager->GetCurrentScene())->GetUIType() == soulBeater::eUIManagerType::InGame)
				{
					mbIsButtonUsable = false;
				}

				mCurrentScene = mSceneManager->GetCurrentScene();
				mUIManagerType = static_cast<soulBeater::SoulBeaterScene*>(mSceneManager->GetCurrentScene())->GetUIType();
			}

			if (d2dFramework::InputManager::GetInstance()->GetKeyState(27) == d2dFramework::eKeyState::Pop)
			{
				if ((mUIManagerType == soulBeater::eUIManagerType::InGame || mUIManagerType == soulBeater::eUIManagerType::InGamePause))
				{
					if (!mbIsButtonUsable)
					{
						mbIsButtonUsable = true;
						mUIManagerType = soulBeater::eUIManagerType::InGamePause;
						mSelected = eButtonType::eButtonTypeEnd;
					}
					else
					{
						FindExactButtonComponent()->OnFocusedOut();
						mbIsButtonUsable = false;
						mUIManagerType = soulBeater::eUIManagerType::InGame;
					}
				}
				else if (mUIManagerType == soulBeater::eUIManagerType::SettingOption)
				{
					if (mbIsButtonUsable)
					{
						FindExactButtonComponent()->OnFocusedOut();
						mUIManagerType = soulBeater::eUIManagerType::InGamePause;
						mSelected = eButtonType::eButtonTypeEnd;
					}
				}
			}

			if (mbIsButtonUsable)
			{
				switch (mUIManagerType)
				{
				case soulBeater::eUIManagerType::StartScene: {updateStartScene(deltaTime); }break;
				case soulBeater::eUIManagerType::InGamePause: {updateInGameOption(deltaTime); }break;
				case soulBeater::eUIManagerType::SettingOption: {updateSettingOption(deltaTime); }break;
				}
			}
		}
		else
		{
			mCurrentScene = mSceneManager->GetCurrentScene();
		}
		for (auto ButtonRenderor : mButtonIDs)
		{
			if (ButtonRenderor.second == mUIManagerType)
			{
				FindObject(ButtonRenderor.first)->GetComponent<d2dFramework::SpriteRenderer>()->SetIsActive(true);
			}
			else
			{
				FindObject(ButtonRenderor.first)->GetComponent<d2dFramework::SpriteRenderer>()->SetIsActive(false);
			}
		}
		for (auto UISprit : mSpritIDs)
		{
			if (FindObjectUIComponent(UISprit.first)->GetUIType() == eUISpriteType::InGameUI && (mUIManagerType == soulBeater::eUIManagerType::SettingOption || mUIManagerType == soulBeater::eUIManagerType::InGamePause || mUIManagerType == soulBeater::eUIManagerType::InGame))
			{
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			}
			else if (UISprit.second == mUIManagerType)
			{
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			}
			else
			{
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(false);
			}
		}
	}

	void UIManager::updateStartScene(float deltaTime)
	{
		if (mSelected < eButtonType::StartButton || mSelected >eButtonType::ExitButton)
		{
			mSelected = eButtonType::StartButton;
			FindExactButtonComponent()->OnFocused();
		}

		if (InputManager::GetInstance()->GetKeyState('S') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected + 1), eButtonType::StartButton, eButtonType::ExitButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState('W') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected - 1), eButtonType::StartButton, eButtonType::ExitButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnPushed();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Pop)
		{
			FindExactButtonComponent()->OnPoped();
		}

	}



	void UIManager::updateInGameOption(float deltaTime)
	{
		if (mSelected < eButtonType::ResumeButton || mSelected >eButtonType::MenuButton)
		{
			mSelected = eButtonType::ResumeButton;
			FindExactButtonComponent()->OnFocused();
		}

		if (InputManager::GetInstance()->GetKeyState('S') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected + 1), eButtonType::ResumeButton, eButtonType::MenuButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState('W') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected - 1), eButtonType::ResumeButton, eButtonType::MenuButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnPushed();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Pop)
		{
			FindExactButtonComponent()->OnPoped();

		}
	}

	void UIManager::updateSettingOption(float deltaTime)
	{
		if (mSelected < eButtonType::SoundButton || mSelected >eButtonType::MusicButton)
		{
			mSelected = eButtonType::SoundButton;
			FindExactButtonComponent()->OnFocused();

		}
		if (InputManager::GetInstance()->GetKeyState('S') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected + 1), eButtonType::SoundButton, eButtonType::MusicButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState('W') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected - 1), eButtonType::SoundButton, eButtonType::MusicButton);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnPushed();
		}
		else if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Pop)
		{
			FindExactButtonComponent()->OnPoped();
		}
	}

	ButtonComponent* UIManager::FindExactButtonComponent()
	{
		for (const auto& pair : mButtonIDs)
		{
			if (/*pair.second == mUIManagerType &&*/
				FindObjectButtonComponent(pair.first)->GetmButtonType() == mSelected)
			{
				return FindObjectButtonComponent(pair.first);
			}
		}
		///nullptr일 때 터뜨리고싶다.
	}

	void UIManager::ButtonSetting(GameObject* button)
	{
		int compId = button->GetId() * 10;
		button->CreateComponent<ButtonComponent>(compId++);
		button->CreateComponent<Transform>(compId++);
		button->CreateComponent<SpriteRenderer>(compId++);
		button->GetComponent<SpriteRenderer>()->SetIsActive(false);
		button->GetComponent<SpriteRenderer>()->SetSpriteType(eSpriteType::Rectangle);
		button->GetComponent<SpriteRenderer>()->SetTransformLayer(eTransformLayer::UI);
		button->GetComponent<Transform>()->SetScale({ 1,-1 });
		button->SetObjectType(eObjectType::Button);
	}

	void UIManager::UISetting(GameObject* UIObject)
	{
		int comID = UIObject->GetId() * 10;
		UIObject->CreateComponent<Transform>(comID++);
		UIObject->CreateComponent<SpriteRenderer>(comID++);
		UIObject->CreateComponent<UIComponent>(comID++);
		UIObject->SetObjectType(eObjectType::UI);
		UIObject->GetComponent<SpriteRenderer>()->SetTransformLayer(eTransformLayer::UI);
		UIObject->GetComponent<Transform>()->SetScale({ 1,-1 });
	}
	void UIManager::TimerUISetting(GameObject* UIObject)
	{
		int comID = UIObject->GetId() * 10;
		UIObject->CreateComponent<Transform>(comID++);
		UIObject->CreateComponent<TimerComponent>(comID++);
		UIObject->SetObjectType(eObjectType::UI);
		UIObject->GetComponent<TimerComponent>()->SetTransformLayer(eTransformLayer::UI);
		UIObject->GetComponent<Transform>()->SetScale({ 1,-1 });
	}

}
