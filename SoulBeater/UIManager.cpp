#include "UIManager.h"

#include "UIComponent.h"
#include "TimerComponent.h"
#include "HPComponent.h"
#include "ButtonComponent.h"
#include "eFrameworkID.h"
#include "EventManager.h"
#include "InputManager.h"
#include "MathHelper.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SoulBeaterScene.h"
#include "eButtonType.h"
#include "GameObject.h"
#include "eSoulBeaterID.h"

namespace soulBeater
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
		using namespace d2dFramework;

		mSceneManager = sceneManager;
		auto registerButton = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			d2dFramework::GameObject* button = d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
			if (button != nullptr)
			{
				assert(button->GetComponent<d2dFramework::Transform>() != nullptr);
				assert(button->GetComponent<ButtonComponent>() != nullptr);
				assert(button->GetComponent<d2dFramework::SpriteRenderer>() != nullptr);
				mButtonIDs.insert({ id, button->GetComponent<ButtonComponent>()->GetOwnManagerType() });
				button->GetComponent<ButtonComponent>()->SetUIManager(this);
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
				assert(UISprite->GetComponent<UIComponent>() != nullptr);
				mSpritIDs.insert({ id, UISprite->GetComponent<UIComponent>()->GetOwnManagerType() });
				UISprite->GetComponent<UIComponent>()->SetUIManager(this);
			}
		};
		auto SetUISceneManager = [this](const std::string& data) -> void
		{
			const unsigned int id = std::stoi(data);

			d2dFramework::GameObject* object = d2dFramework::ObjectManager::GetInstance()->FindObjectOrNull(id, true);
			if (object != nullptr)
			{
				assert(object->GetComponent<d2dFramework::Transform>() != nullptr);
				if (object->GetComponent<TimerComponent>() != nullptr)
				{

					mTimer = object;
					assert(object->GetComponent<HPComponent>() == nullptr);
				}
				else if (object->GetComponent<HPComponent>() != nullptr)
				{
					assert(object->GetComponent<TimerComponent>() == nullptr);
				}

			}
		};
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("RegisterButtonToManager", GetId(), registerButton);
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("RegisterUISpriteToManager", GetId(), registerUI);
		d2dFramework::EventManager::GetInstance()->RegisterEventHandler("SetUISceneManager", GetId(), SetUISceneManager);

#pragma region UI생성


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
		GameObject* buttonTomain = ObjectManager::GetInstance()->CreateObject(517, true);
		GameObject* buttonGameOverRestart = ObjectManager::GetInstance()->CreateObject(520, true);
		GameObject* buttonGameOverExit = ObjectManager::GetInstance()->CreateObject(521, true);
		
#pragma endregion

#pragma region UI create
		GameObject* UIStart = ObjectManager::GetInstance()->CreateObject(516, true);
		GameObject* UIIngame = ObjectManager::GetInstance()->CreateObject(509, true);
		GameObject* HPRender = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::HPObject), true);
		GameObject* UITimer = ObjectManager::GetInstance()->CreateObject(static_cast<unsigned int>(eSoulBeaterID::TimerObject), true);
		GameObject* UIIngameOption = ObjectManager::GetInstance()->CreateObject(510, true);
		GameObject* UISettiongOption = ObjectManager::GetInstance()->CreateObject(511, true);
		GameObject* UISoundVolume = ObjectManager::GetInstance()->CreateObject(512, true);
		GameObject* UIMusicVolume = ObjectManager::GetInstance()->CreateObject(513, true);
		GameObject* UIHelp = ObjectManager::GetInstance()->CreateObject(518, true);
		GameObject* UIGameOver = ObjectManager::GetInstance()->CreateObject(522, true);
		GameObject* UIInBoss = ObjectManager::GetInstance()->CreateObject(523, true);

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

		ButtonSetting(buttonTomain);
		buttonTomain->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::HelpOutButton);
		buttonTomain->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::HelpScene);
		buttonTomain->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"HelpOutButton"));

		ButtonSetting(buttonGameOverRestart);
		buttonGameOverRestart->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::GameOverReStart);
		buttonGameOverRestart->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::GameOver);
		buttonGameOverRestart->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"GameOverRestart"));

		ButtonSetting(buttonGameOverExit);
		buttonGameOverExit->GetComponent<ButtonComponent>()->SetmButtonType(eButtonType::GameOverExit);
		buttonGameOverExit->GetComponent<ButtonComponent>()->SetmOwnManagerType(soulBeater::eUIManagerType::GameOver);
		buttonGameOverExit->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"GameOverExit"));
		///버튼들 위치 잡기
		buttonStart->GetComponent<Transform>()->SetTranslate({ 538,785 });
		buttonHelp->GetComponent<Transform>()->SetTranslate({ 538, 871 });
		buttonExit->GetComponent<Transform>()->SetTranslate({ 538, 948 });
		buttonResume->GetComponent<Transform>()->SetTranslate({ 960, 453 });
		buttonSetting->GetComponent<Transform>()->SetTranslate({ 960, 547 });
		buttonMenu->GetComponent<Transform>()->SetTranslate({ 962, 641 });
		buttonSound->GetComponent<Transform>()->SetTranslate({ 865, 490 });
		buttonMusic->GetComponent<Transform>()->SetTranslate({ 896,589 });
		buttonTomain->GetComponent<Transform>()->SetTranslate({ 110, 91 });
		buttonGameOverRestart->GetComponent<Transform>()->SetTranslate({ 624 ,970 });
		buttonGameOverExit->GetComponent<Transform>()->SetTranslate({ 1295 ,970 });
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

		UISetting(UIHelp);
		UIHelp->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::HelpScene);
		UIHelp->GetComponent<UIComponent>()->SetUIType(eUISpriteType::HelpUI);
		UIHelp->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"HelpUI"));
		UIHelp->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });

		UISetting(UIGameOver);
		UIGameOver->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::GameOver);
		UIGameOver->GetComponent<UIComponent>()->SetUIType(eUISpriteType::GameOverUI);
		UIGameOver->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"GameOverUI"));
		UIGameOver->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });

		UISetting(UIInBoss);
		UIInBoss->GetComponent<UIComponent>()->SetOwnManagerType(soulBeater::eUIManagerType::InBoss);
		UIInBoss->GetComponent<UIComponent>()->SetUIType(eUISpriteType::InBossUI);
		UIInBoss->GetComponent<SpriteRenderer>()->SetBitmap(RenderManager::GetInstance()->GetBitmapOrNull(L"InBossUI"));
		UIInBoss->GetComponent<Transform>()->SetTranslate({ 1920 / 2,1080 / 2 });




		TimerUISetting(UITimer);
		UITimer->GetComponent<Transform>()->SetTranslate({ 270,90 });
		mTimer = UITimer;
		HPRender->CreateComponent<Transform>(5151);
		HPComponent* hpComp = HPRender->CreateComponent<HPComponent>(5152);
		hpComp->SetIsActive(false);
		HPRender->SetObjectType(eObjectType::UI);
		HPRender->GetComponent<HPComponent>()->SetTransformLayer(eTransformLayer::UI);
		HPRender->GetComponent<Transform>()->SetScale({ 1,-1 });
		HPRender->GetComponent<Transform>()->SetTranslate({ 287,246 });
#pragma endregion

	}

	void UIManager::Release()
	{
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("RegisterButtonToManager", GetId());
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("RegisterUISpriteToManager", GetId());
		d2dFramework::EventManager::GetInstance()->UnregisterEventHandler("SetUISceneManager", GetId());
	}

	void UIManager::Update(float deltaTime)
	{
		//test
		if (mCurrentScene != nullptr)
		{
			if (mCurrentScene != mSceneManager->GetCurrentScene())
			{
				soulBeater::eUIManagerType mapManagerType = static_cast<soulBeater::SoulBeaterScene*>(mSceneManager->GetCurrentScene())->GetUIType();

				if (mapManagerType == soulBeater::eUIManagerType::InGame)
					mbIsButtonUsable = false;
				else if(mapManagerType == soulBeater::eUIManagerType::None)
					mbIsButtonUsable = false;
				else
					mbIsButtonUsable = true;

				mCurrentScene = mSceneManager->GetCurrentScene();
				mUIManagerType = mapManagerType;
				//test
				eUIManagerType type = GetUIManagerType();
				if (mTimer != nullptr && (type == soulBeater::eUIManagerType::InGame
					|| type == soulBeater::eUIManagerType::InGamePause
					|| type == soulBeater::eUIManagerType::SettingOption))
				{
					mTimer->GetComponent<TimerComponent>()->SetIsActive(true);
				}
				else
				{
					mTimer->GetComponent<TimerComponent>()->SetIsActive(false);
				}
			}

			if (d2dFramework::InputManager::GetInstance()->GetKeyState(VK_ESCAPE) == d2dFramework::eKeyState::Pop)
			{
				if ((mUIManagerType == soulBeater::eUIManagerType::InGame || mUIManagerType == soulBeater::eUIManagerType::InGamePause))
				{

					if (!mbIsButtonUsable)
					{
					d2dFramework::SoundManager::GetInstance()->Pause(d2dFramework::eSoundType::BGM, 0, true);
						mbIsButtonUsable = true;
						mUIManagerType = soulBeater::eUIManagerType::InGamePause;
						mSelected = eButtonType::eButtonTypeEnd;
					}
					else
					{
					d2dFramework::SoundManager::GetInstance()->Pause(d2dFramework::eSoundType::BGM, 0, false);
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
				case soulBeater::eUIManagerType::StartScene:
					updateStartScene(deltaTime);
					break;
				case eUIManagerType::HelpScene: 
					updateHelpsScene(deltaTime);
					break;
				case eUIManagerType::InGame: break;
				case soulBeater::eUIManagerType::InGamePause:
					updateInGameOption(deltaTime);
					break;
				case soulBeater::eUIManagerType::SettingOption:
					updateSettingOption(deltaTime);
					break;
				case eUIManagerType::EndingScene: break;
				case eUIManagerType::None: break;
				case eUIManagerType::GameOver:
					updateGameOverScene(deltaTime);
					break;
				default: ;
				}
			}
		}
		else
		{
			mCurrentScene = mSceneManager->GetCurrentScene();
		}

		for (auto& ButtonRenderor : mButtonIDs)
		{
			if (ButtonRenderor.second == mUIManagerType)
			{
				FindObjectSpriteRenderer(ButtonRenderor.first)->SetIsActive(true);
			}
			else if(ButtonRenderor.second == eUIManagerType::StartScene&& mUIManagerType==eUIManagerType::HelpScene)
			{
				FindObjectSpriteRenderer(ButtonRenderor.first)->SetIsActive(false);
			}
			else
			{
				FindObjectSpriteRenderer(ButtonRenderor.first)->SetIsActive(false);
			}
		}

		for (auto& UISprit : mSpritIDs)
		{
			SoulBeaterScene* map = static_cast<soulBeater::SoulBeaterScene*>(mCurrentScene);
			eUISpriteType UISpritUIType = FindObjectUIComponent(UISprit.first)->GetUIType();

			eSceneType mapSceneType= map->GetSceneType();
			eUIManagerType mapManagerType= map->GetUIType();

			bool IngameUI=false;
			bool InBossUI= false;

			switch (mapSceneType)
			{
			case eSceneType::Start: break;
			case eSceneType::Ending: break;
			case eSceneType::CutScene1: break;
			case eSceneType::CutScene2: break;
			case eSceneType::CutScene3: break;
			case eSceneType::Stage1_1:;
			case eSceneType::Stage1_2:;
			case eSceneType::Stage1_3:;
			case eSceneType::Stage2_1:;
			case eSceneType::Stage2_2:;
			case eSceneType::Stage2_3:;
			case eSceneType::Stage3_1:;
			case eSceneType::Stage3_2:;
			case eSceneType::Stage3_3: {IngameUI = true; }break;
			case eSceneType::Boss1:;
			case eSceneType::Boss2:;
			case eSceneType::Boss3: {InBossUI = true; }break;
			case eSceneType::Size: break;
			default:break;
			}

			if (UISpritUIType == eUISpriteType::InGameUI && mapManagerType == eUIManagerType::InGame && IngameUI == true)
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			else if (UISpritUIType == eUISpriteType::InBossUI && mapManagerType == eUIManagerType::InGame && InBossUI == true)
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			else if (UISpritUIType == eUISpriteType::StartUI && (mUIManagerType == eUIManagerType::HelpScene))
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			else if (UISprit.second == mUIManagerType && UISpritUIType != InGameUI)
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(true);
			else
				FindObjectSpriteRenderer(UISprit.first)->SetIsActive(false);

		}
	}



	void UIManager::updateStartScene(float deltaTime)
	{
		using namespace d2dFramework;

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

	void UIManager::updateHelpsScene(float deltaTime)
	{
		using namespace d2dFramework;
		if (mSelected < eButtonType::HelpOutButton || mSelected >eButtonType::HelpOutButton)
		{
			mSelected = eButtonType::HelpOutButton;
			FindExactButtonComponent()->OnFocused();
		}


		if (InputManager::GetInstance()->GetKeyState(' ') == eKeyState::Push)
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
		using namespace d2dFramework;

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
		using namespace d2dFramework;

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

	void UIManager::updateGameOverScene(float deltaTime)
	{
		using namespace d2dFramework;

		if (mSelected < eButtonType::GameOverReStart || mSelected >eButtonType::GameOverExit)
		{
			mSelected = eButtonType::GameOverReStart;
			FindExactButtonComponent()->OnFocused();

		}
		if (InputManager::GetInstance()->GetKeyState('D') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected + 1), eButtonType::GameOverReStart, eButtonType::GameOverExit);
			FindExactButtonComponent()->OnFocused();
		}
		else if (InputManager::GetInstance()->GetKeyState('A') == eKeyState::Push)
		{
			FindExactButtonComponent()->OnFocusedOut();
			mSelected = (eButtonType)MathHelper::Clamp((float)((int)mSelected - 1), eButtonType::GameOverReStart, eButtonType::GameOverExit);
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

	void UIManager::ButtonSetting(d2dFramework::GameObject* button)
	{
		using namespace d2dFramework;

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

	void UIManager::UISetting(d2dFramework::GameObject* UIObject)
	{
		using namespace d2dFramework;

		int comID = UIObject->GetId() * 10;
		UIObject->CreateComponent<Transform>(comID++);
		UIObject->CreateComponent<SpriteRenderer>(comID++);
		UIObject->CreateComponent<UIComponent>(comID++);
		UIObject->SetObjectType(eObjectType::UI);
		UIObject->GetComponent<SpriteRenderer>()->SetTransformLayer(eTransformLayer::UI);
		UIObject->GetComponent<Transform>()->SetScale({ 1,-1 });
	}
	void UIManager::TimerUISetting(d2dFramework::GameObject* UIObject)
	{
		using namespace d2dFramework;

		int comID = UIObject->GetId() * 10;
		UIObject->CreateComponent<Transform>(comID++);
		UIObject->CreateComponent<TimerComponent>(comID++);
		UIObject->SetObjectType(eObjectType::UI);
		UIObject->GetComponent<TimerComponent>()->SetTransformLayer(eTransformLayer::UI);
		UIObject->GetComponent<Transform>()->SetScale({ 1,-1 });
	}
}
