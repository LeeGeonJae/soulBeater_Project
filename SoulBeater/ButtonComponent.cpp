#include "ButtonComponent.h"

#include "eSoulBeaterID.h"
#include "EventManager.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "IncludeComponent.h"
#include "UIManager.h"
#include "UIComponent.h"
#include "GameObject.h"

namespace soulBeater
{
	ButtonComponent::ButtonComponent(unsigned int id, d2dFramework::GameObject* owner)
		:Component(id, owner)
		, mbButtonPushed()
		, mButtonType()
		, mOwnUIManagerType()
		, mButtonUVRectSizes()
		, mUIManager(nullptr)
		, mBitMapkey(nullptr)
	{
	}

	void ButtonComponent::Init()
	{
		using namespace d2dFramework;

		GameObject* gameObject = GetGameObject();

		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterButtonToManager", std::to_string(gameObject->GetId()));
		SpriteRenderer* renderer = gameObject->GetComponent<SpriteRenderer>();

		renderer->SetIsActive(false);
		renderer->SetSpriteType(eSpriteType::Sprite);

		ID2D1Bitmap* bitmap = renderer->GetBitmap();

		float spriteWidth = static_cast<float>(bitmap->GetPixelSize().width) / 3.f;
		float spriteHeight = static_cast<float>(bitmap->GetPixelSize().height);

		mButtonUVRectSizes[eButtonStatus::FocusOut] = { 0,0,spriteWidth ,spriteHeight };
		mButtonUVRectSizes[eButtonStatus::FocusOn] = { spriteWidth,0,spriteWidth * 2,spriteHeight };
		mButtonUVRectSizes[eButtonStatus::Pushed] = { spriteWidth * 2,0,spriteWidth * 3,spriteHeight };

		renderer->SetSize({ spriteWidth,spriteHeight });
		renderer->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOut]);
	}

	void ButtonComponent::Release()
	{
	}

	void ButtonComponent::OnPushed()
	{
		using namespace d2dFramework;

		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::Pushed]);

		mbButtonPushed = true;
		switch (mButtonType)
		{
		case eButtonType::StartButton: {}break;
		case eButtonType::HelpButton: {}break;
		case eButtonType::ExitButton: {}break;
		case eButtonType::ResumeButton: {}break;
		case eButtonType::SettingButton: {}break;
		case eButtonType::MenuButton: {}break;
		case eButtonType::SoundButton:
		{
			if (SoundManager::GetInstance()->GetEffectVolume() > 0.5f)
				SoundManager::GetInstance()->SetEffectsVolume(0.4f);
			else if (SoundManager::GetInstance()->GetEffectVolume() > 0.3f)
				SoundManager::GetInstance()->SetEffectsVolume(0.2f);
			else if (SoundManager::GetInstance()->GetEffectVolume() > 0.1f)
				SoundManager::GetInstance()->SetEffectsVolume(0.f);
			else if (SoundManager::GetInstance()->GetEffectVolume() < 0.1f)
				SoundManager::GetInstance()->SetEffectsVolume(0.6f);
		}break;
		case eButtonType::MusicButton:
		{
			if (SoundManager::GetInstance()->GetBgmVolume() > 0.9f)
				SoundManager::GetInstance()->SetBGMVolume(0.6f);
			else if (SoundManager::GetInstance()->GetBgmVolume() > 0.5f)
				SoundManager::GetInstance()->SetBGMVolume(0.4f);
			else if (SoundManager::GetInstance()->GetBgmVolume() > 0.3f)
				SoundManager::GetInstance()->SetBGMVolume(0.f);
			else if (SoundManager::GetInstance()->GetBgmVolume() < 0.1f)
				SoundManager::GetInstance()->SetBGMVolume(1.f);

		}break;
		case HelpOutButton: break;
		case GameOverReStart: break;
		case GameOverExit: break;
		case eButtonTypeEnd: break;
		default: ;
		}
	}

	void ButtonComponent::OnPoped()
	{
		using namespace d2dFramework;

		if (mbButtonPushed)
		{
			GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOn]);
			switch (mButtonType)
			{
			case eButtonType::StartButton:
				{
					EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", std::to_string(static_cast<unsigned int>(eSoulBeaterID::Stage1_1)));/*씬체인지*/
				}
				break;
			case eButtonType::HelpButton:
				{
					GetUIManager()->SetmButtonManagerType(eUIManagerType::HelpScene);
					OnFocusedOut();
				}break;
			case HelpOutButton:
				{
					GetUIManager()->SetmButtonManagerType(eUIManagerType::StartScene);
					OnFocusedOut();
				}break;
			case eButtonType::ExitButton:
				{
					///게임 끄기
					PostQuitMessage(0);
				}break;
			case eButtonType::ResumeButton:
				///노래 및 게임 재 실행 기능 만들기
				break;
			case eButtonType::SettingButton:
				{
					/// 설정키 누를 시 다른 UI 버튼 뜨도록 하기 + 버튼 매니져컴프에서 button 렌더 관리?
					GetUIManager()->SetmButtonManagerType(eUIManagerType::SettingOption);
					OnFocusedOut();
				}break;
			case eButtonType::MenuButton:
				{
					///타이틀 SceneID 값 넣기
					OnFocusedOut();
					EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", std::to_string(static_cast<unsigned int>(eSoulBeaterID::StartScene)));/*씬체인지*/
				}break;
			case eButtonType::SoundButton:
				{
					GameObject* SoundVolume = ObjectManager::GetInstance()->FindObjectOrNull(512, true);
					if (SoundManager::GetInstance()->GetEffectVolume() > 0.5f)
						SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(0));
					else if (SoundManager::GetInstance()->GetEffectVolume() > 0.3f)
						SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(1));
					else if (SoundManager::GetInstance()->GetEffectVolume() > 0.1f)
						SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(2));
					else if (SoundManager::GetInstance()->GetEffectVolume() < 0.1f)
						SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(3));
				}break;
			case eButtonType::MusicButton:
				{
					GameObject* MusicVolume = ObjectManager::GetInstance()->FindObjectOrNull(513, true);
					if (SoundManager::GetInstance()->GetBgmVolume() > 0.9f)
						MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(0));
					else if (SoundManager::GetInstance()->GetBgmVolume() > 0.5f)
						MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(1));
					else if (SoundManager::GetInstance()->GetBgmVolume() > 0.3f)
						MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(2));
					else if (SoundManager::GetInstance()->GetBgmVolume() < 0.1f)
						MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(3));
				}break;
			case GameOverReStart:
				{
					EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", std::to_string(static_cast<unsigned int>(eSoulBeaterID::StartScene)));/*씬체인지*/
				}break;
			case GameOverExit:
				{
					PostQuitMessage(0);
				}break;
		case eButtonTypeEnd: break;

		default: ;
			}
			mbButtonPushed = false;
		}
	}

	void ButtonComponent::OnFocused()
	{
		using namespace d2dFramework;

		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOn]);
	}

	void ButtonComponent::OnFocusedOut()
	{
		using namespace d2dFramework;

		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOut]);
		mbButtonPushed = false;
	}

#pragma region Serialize
	void ButtonComponent::SerializeIn(nlohmann::ordered_json& object)
	{
		mButtonType = object["mButtonType"];
		mOwnUIManagerType = object["OwnButtonManagerType"];

		for (auto& jsonUVRectSize : object["ButtonUVRectSizes"])
		{
			int ButtonStatus = jsonUVRectSize["eButtonStatus"];
			mButtonUVRectSizes[ButtonStatus].left = jsonUVRectSize["UVRectSize_left"];
			mButtonUVRectSizes[ButtonStatus].top = jsonUVRectSize["UVRectSize_top"];
			mButtonUVRectSizes[ButtonStatus].right = jsonUVRectSize["UVRectSize_right"];
			mButtonUVRectSizes[ButtonStatus].bottom = jsonUVRectSize["UVRectSize_bottom"];
		}
	}

	void ButtonComponent::SerializeOut(nlohmann::ordered_json& object)
	{
		object["ComponentName"] = "ButtonComponent";
		Component::SerializeOut(object);
		object["mButtonType"] = mButtonType;
		object["OwnButtonManagerType"] = mOwnUIManagerType;
		for (size_t i = 0; i < eButtonStatus::Pushed; i++)
		{
			nlohmann::ordered_json jsonButtonUVRectSizes;
			jsonButtonUVRectSizes["eButtonStatus"] = i;
			jsonButtonUVRectSizes["UVRectSize_left"] = mButtonUVRectSizes[i].left;
			jsonButtonUVRectSizes["UVRectSize_top"] = mButtonUVRectSizes[i].top;
			jsonButtonUVRectSizes["UVRectSize_right"] = mButtonUVRectSizes[i].right;
			jsonButtonUVRectSizes["UVRectSize_bottom"] = mButtonUVRectSizes[i].bottom;
			object["ButtonUVRectSizes"].push_back(jsonButtonUVRectSizes);
		}
	}
#pragma endregion
}

