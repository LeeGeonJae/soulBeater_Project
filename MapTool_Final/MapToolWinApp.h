#pragma once

#include <Windows.h>

namespace d2dFramework
{
	class GameProcessor;
}

namespace mapTool
{
	class MapToolWinApp
	{
	public:
		static int Run(d2dFramework::GameProcessor* pSample, HINSTANCE hInstance, int nCmdShow);

		static HWND GetHwnd();
		static RECT GetWindowRect();

	protected:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		static void placeInCenterOfScreen(HWND window, DWORD style, DWORD exStyle);

		MapToolWinApp() = delete;
		~MapToolWinApp() = delete;

	private:
		static HWND mHwnd;
		static RECT windowRect;
	};
}