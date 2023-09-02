#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "eUISpriteType.h"
#include "eUIManagerType.h"

#include <d2d1.h>
#include <d2d1_1.h>
#include <array>

namespace soulBeater
{
	class UIManager;

	class UIComponent : public d2dFramework::Component
	{
	public:
		UIComponent(unsigned int id, d2dFramework::GameObject* owner);
		~UIComponent() override;
		void Init() override;
		void Release() override;

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		void SetmUIType(eUISpriteType uiType) { mUIType = uiType; }
		inline void SetUIManager(UIManager* UIManager) { mUIManager = UIManager; }
		inline void SetOwnManagerType(eUIManagerType ManagerType) { mOwnUIManagerType = ManagerType; }
		inline void SetUIType(eUISpriteType uiType) { mUIType = uiType; }

		inline UIManager* GetUIManager() { return mUIManager; }
		inline eUISpriteType GetUIType() { return mUIType; }
		inline eUIManagerType GetOwnManagerType() { return mOwnUIManagerType; }
		inline D2D_RECT_F GetmSoundMusicRect(int index) { return mSoundMusicRect[index]; }

	private:
		UIManager* mUIManager;
		eUIManagerType mOwnUIManagerType;
		eUISpriteType mUIType;
		D2D_RECT_F mUIRect;
		std::array<D2D_RECT_F, 4 > mSoundMusicRect;
	};
}

