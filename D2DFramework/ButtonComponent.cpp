#include "ButtonComponent.h"

#include "EventManager.h"
#include "GameObject.h"
#include "ObjectManager.h"
#include "RenderManger.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "SpriteRenderer.h"
#include "UIManager.h"

namespace d2dFramework
{
	class SpriteRenderer;


	ButtonComponent::ButtonComponent(unsigned int id, GameObject* owner)
		:Component(id,owner)
	{
	}


	void ButtonComponent::Init()
	{
		EventManager::GetInstance()->RegisterBroadcastEvent("RegisterButtonToManager", std::to_string(this->GetGameObject()->GetId()));
		GetGameObject()->GetComponent<SpriteRenderer>()->SetIsActive(false);
		
			GetGameObject()->GetComponent<SpriteRenderer>()->SetSpriteType(eSpriteType::Sprite);
			float spriteWidth = GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetPixelSize().width / 3;
			float spriteHeight = GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetPixelSize().height;
			mButtonUVRectSizes[eButtonStatus::FocusOut] = { 0,0,spriteWidth ,spriteHeight };
			mButtonUVRectSizes[eButtonStatus::FocusOn] = { spriteWidth,0,spriteWidth * 2,spriteHeight };
			mButtonUVRectSizes[eButtonStatus::Pushed] = { spriteWidth * 2,0,spriteWidth * 3,spriteHeight };
			
			GetGameObject()->GetComponent<SpriteRenderer>()->SetSize({ spriteWidth,spriteHeight });
			GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOut]);


	}
	void ButtonComponent::Release()
	{
	}

	void ButtonComponent::OnPushed()
	{
		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::Pushed]);

		mbButtonPushed = true;
		switch (mButtonType)
		{
		case eButtonType::StartButton:	{}break;
		case eButtonType::HelpButton:	{}break;
		case eButtonType::ExitButton:	{}break;
		case eButtonType::ResumeButton:	{}break;
		case eButtonType::SettingButton:{}break;
		case eButtonType::MenuButton:	{}break;
		case eButtonType::SoundButton:
		{
			if (SoundManager::GetInstance()->GetEffectVolume() > 0.9f)
				SoundManager::GetInstance()->SetEffectsVolume(0.6f);
			else if (SoundManager::GetInstance()->GetEffectVolume() > 0.5f)
				SoundManager::GetInstance()->SetEffectsVolume(0.4f);
			else if (SoundManager::GetInstance()->GetEffectVolume() > 0.3f)
				SoundManager::GetInstance()->SetEffectsVolume(0.f);
			else if (SoundManager::GetInstance()->GetEffectVolume() < 0.1f)
				SoundManager::GetInstance()->SetEffectsVolume(1.f);
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

		}
	}

	void ButtonComponent::OnPoped()
	{
		if (mbButtonPushed)
		{
			GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOn]);
			switch (mButtonType)
			{
 			case eButtonType::StartButton: {EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", "702");/*씬체인지*/}break;
			case eButtonType::HelpButton:
			{}break;
			case eButtonType::ExitButton:
			{
				///게임 끄기
				PostQuitMessage(0);
			}break;
			case eButtonType::ResumeButton:
			{
				///노래 및 게임 재 실행 기능 만들기
			}break;
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
 				EventManager::GetInstance()->ExcuteBroadcastEvent("ChangeScene", "701");/*씬체인지*/
			}break;
			case eButtonType::SoundButton:
				{
				GameObject* SoundVolume = ObjectManager::GetInstance()->FindObjectOrNull(512, true);
				if (SoundManager::GetInstance()->GetEffectVolume() > 0.9f)
					SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(1));
				else if (SoundManager::GetInstance()->GetEffectVolume() > 0.5f)
					SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(2));
				else if (SoundManager::GetInstance()->GetEffectVolume() > 0.3f)
					SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(3));
				else if (SoundManager::GetInstance()->GetEffectVolume() < 0.1f)
					SoundVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(SoundVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(0));
				}break;
			case eButtonType::MusicButton:
				{
				GameObject* MusicVolume =ObjectManager::GetInstance()->FindObjectOrNull(513, true);
				if (SoundManager::GetInstance()->GetBgmVolume() > 0.9f)
					MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(1));
				else if (SoundManager::GetInstance()->GetBgmVolume() > 0.5f)
					MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(2));
				else if (SoundManager::GetInstance()->GetBgmVolume() > 0.3f)
					MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(3));
				else if (SoundManager::GetInstance()->GetBgmVolume() < 0.1f)
					MusicVolume->GetComponent<SpriteRenderer>()->SetUVRectangle(MusicVolume->GetComponent<UIComponent>()->GetmSoundMusicRect(0));
				}break;

			}
			mbButtonPushed = false;
		}
	}

	void ButtonComponent::OnFocused()
	{
		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOn]);
	}

	void ButtonComponent::OnFocusedOut()
	{
		GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mButtonUVRectSizes[eButtonStatus::FocusOut]);
		mbButtonPushed = false;
	}

#pragma region Serialize
	void ButtonComponent::SerializeIn(nlohmann::ordered_json& object)
	{
		mButtonType = object["mButtonType"];
		mOwnUIManagerType= object["OwnButtonManagerType"];

		for (auto jsonUVRectSize : object["ButtonUVRectSizes"])
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

