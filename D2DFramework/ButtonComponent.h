#pragma once
#include <d2d1.h>
#include <d2d1_1.h>
#include <array>

#include "Component.h"
#include "EnumButtons.h"
#include "IUpdateable.h"

namespace d2dFramework
{
	class UIManager;
	/*
	 Component for Remember Button Information and Acting
	 */
	class ButtonComponent : public Component
	{

	public:
		ButtonComponent(unsigned int id, GameObject* owner);
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
		void SetmOwnManagerType(eUIManagerType managerType) {mOwnUIManagerType = managerType;}
		void SetUIManager(UIManager* UIManager) { mUIManager = UIManager; }
		void SetmButtonUVRectSize(D2D_RECT_F size , eButtonStatus status)
		{mButtonUVRectSizes[status] = size;}
		void SetmBitMapkey(){}
		eUIManagerType GetOwnManagerType() { return mOwnUIManagerType;}
		eButtonType GetmButtonType() {return mButtonType;	}
		UIManager* GetUIManager() { return mUIManager; }

	private:
		bool mbButtonPushed;
		eButtonType mButtonType;
		eUIManagerType mOwnUIManagerType;
		std::array <D2D_RECT_F, 3 > mButtonUVRectSizes;
		UIManager* mUIManager=nullptr;
		WCHAR* mBitMapkey;
	};

}
