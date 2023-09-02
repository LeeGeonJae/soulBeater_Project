#pragma once
#include <d2d1.h>
#include <d2d1_1.h>
#include <array>

#include "Component.h"
#include "EnumButtons.h"
#include "IUpdateable.h"

namespace soulBeater
{
	enum class eUIManagerType;
}

namespace  d2dFramework
{
	class UIManager;
	/*
	 Component for Remember UI Information
	 */
	class UIComponent :
    public Component
{
public:
	UIComponent(unsigned int id, GameObject* owner);
	~UIComponent() override;
	void Init() override;
	void Release() override;

	void SerializeIn(nlohmann::ordered_json& object) override;
	void SerializeOut(nlohmann::ordered_json& object) override;

	//void SetmUIType(eUISpriteType uiType){ mUIType = uiType; }
	void SetUIManager(UIManager* UIManager) { mUIManager = UIManager; }
	void SetOwnManagerType(soulBeater::eUIManagerType ManagerType) { mOwnUIManagerType = ManagerType; }
	UIManager* GetUIManager() { return mUIManager; }
	void SetUIType(eUISpriteType uiType) {  mUIType= uiType; }
	eUISpriteType GetUIType() { return mUIType; }
	soulBeater::eUIManagerType GetOwnManagerType() { return mOwnUIManagerType; }
	D2D_RECT_F GetmSoundMusicRect(int index) { return mSoundMusicRect[index]; }

private:
	UIManager* mUIManager;
	soulBeater::eUIManagerType mOwnUIManagerType;
	eUISpriteType mUIType;
	D2D_RECT_F mUIRect;
	std::array<D2D_RECT_F, 4 > mSoundMusicRect;
};
	
}

