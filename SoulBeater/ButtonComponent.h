#pragma once

#include "Component.h"
#include "IUpdateable.h"
#include "eButtonType.h"
#include "eButtonstatus.h"
#include "eUIManagerType.h"

#include <d2d1.h>
#include <d2d1_1.h>
#include <array>

namespace soulBeater
{
	class UIManager;
	/*
	 Component for Remember Button Information and Acting
	 */
	class ButtonComponent : public d2dFramework::Component
	{
	public:
		ButtonComponent(unsigned int id, d2dFramework::GameObject* owner);
		~ButtonComponent() override = default;
		void Init() override;
		void Release() override;
		void OnPushed();
		void OnPoped();
		void OnFocused();
		void OnFocusedOut();

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		void SetmButtonType(eButtonType buttonType) { mButtonType = buttonType; }
		void SetmOwnManagerType(eUIManagerType managerType) { mOwnUIManagerType = managerType; }
		void SetUIManager(soulBeater::UIManager* UIManager) { mUIManager = UIManager; }
		void SetmButtonUVRectSize(D2D_RECT_F size, eButtonStatus status)
		{
			mButtonUVRectSizes[status] = size;
		}
		void SetmBitMapkey() {}
		eUIManagerType GetOwnManagerType() { return mOwnUIManagerType; }
		eButtonType GetmButtonType() { return mButtonType; }
		UIManager* GetUIManager() { return mUIManager; }

	private:
		bool						mbButtonPushed;
		eButtonType					mButtonType;
		eUIManagerType				mOwnUIManagerType;
		std::array <D2D_RECT_F, 3 > mButtonUVRectSizes;
		UIManager*					mUIManager = nullptr;
		WCHAR*						mBitMapkey;
	};

}
