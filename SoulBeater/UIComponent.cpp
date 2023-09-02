#include "UIComponent.h"

#include "Component.h"
#include "EventManager.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "UIManager.h"
#include "TimerComponent.h"

namespace soulBeater
{
	UIComponent::UIComponent(unsigned id, d2dFramework::GameObject* owner)
		:Component(id, owner)
	{
	}

	UIComponent::~UIComponent()
	{}

	void UIComponent::Init()
	{
		using namespace d2dFramework;
		///SettingMap 에서 UIRect를 설정.
		if (GetGameObject()->GetComponent<SpriteRenderer>() != nullptr)
		{
			if (GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap() != nullptr && mUIType == eUISpriteType::SoundMusicUI)
			{
				float RectWidth = GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetSize().width / 4;
				float RectHeight = GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetSize().height;

				mSoundMusicRect[0] = { 0,0, RectWidth, RectHeight };
				mSoundMusicRect[1] = { RectWidth,0, RectWidth * 2, RectHeight };
				mSoundMusicRect[2] = { RectWidth * 2,0, RectWidth * 3, RectHeight };
				mSoundMusicRect[3] = { RectWidth * 3,0, RectWidth * 4, RectHeight };
				mUIRect = mSoundMusicRect[0];
				GetGameObject()->GetComponent<SpriteRenderer>()->SetSize({ mUIRect.right,mUIRect.bottom });
			}
			else if (GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap() != nullptr)
			{
				mUIRect = { 0,0,GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetSize().width ,GetGameObject()->GetComponent<SpriteRenderer>()->GetBitmap()->GetSize().height };
				GetGameObject()->GetComponent<SpriteRenderer>()->SetSize({ mUIRect.right,mUIRect.bottom });
			}

			EventManager::GetInstance()->RegisterBroadcastEvent("RegisterUISpriteToManager", std::to_string(this->GetGameObject()->GetId()));
			GetGameObject()->GetComponent<SpriteRenderer>()->SetUVRectangle(mUIRect);
			GetGameObject()->GetComponent<SpriteRenderer>()->SetSpriteType(eSpriteType::Sprite);
			GetGameObject()->GetComponent<SpriteRenderer>()->SetIsActive(false);
		}
		///타이머 기능


	}

	void UIComponent::Release()
	{
	}


	void UIComponent::SerializeIn(nlohmann::ordered_json& object)
	{
	}



	void UIComponent::SerializeOut(nlohmann::ordered_json& object)
	{
		Component::SerializeOut(object);
	}

}

