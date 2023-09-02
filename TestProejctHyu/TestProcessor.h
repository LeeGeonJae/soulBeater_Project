#pragma once
#include "GameProcessor.h"
#include "KeyInformation.h"

#include <queue>
#include <future>
#include <d2d1_1.h>
#include "../TestProejctHyu/UIManager.h"


namespace d2dFramework
{
	class GameObject;
	class AsyncInputManager;
	class BeatManager;
	class GameObject;
}


namespace TestProjectLDH
{
	class TestProjectLDH : public d2dFramework::GameProcessor
	{
	public:
		TestProjectLDH(UINT width, UINT height, std::wstring name);
		virtual ~TestProjectLDH();

		void Init(HWND hwnd) override;
		void Update() override;
		void Release() override;

	private:
		void initAnimationAsset();
		d2dFramework::UIManager* mUIManager;
	private:
	};
}