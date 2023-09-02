#include "defalut.h"
#include "MapToolWinApp.h"
#include "GameProcessor.h"

#include "gui.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace mapTool
{
	HWND MapToolWinApp::mHwnd = NULL;
	RECT MapToolWinApp::windowRect = {};

	int MapToolWinApp::Run(d2dFramework::GameProcessor* gameProcessor, HINSTANCE hInstance, int nCmdShow)
	{
		using namespace d2dFramework;

		WNDCLASSEX windowClass = { 0 };
		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowProc;
		windowClass.hInstance = hInstance;
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.lpszClassName = L"DXSampleClass";
		RegisterClassEx(&windowClass);

		windowRect = { 0, 0, static_cast<LONG>(gameProcessor->GetWidth()), static_cast<LONG>(gameProcessor->GetHeight()) };
		AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

		mHwnd = CreateWindow(
			windowClass.lpszClassName,
			gameProcessor->GetTitle(),
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			nullptr,
			nullptr,
			hInstance,
			gameProcessor);

		gameProcessor->Init(mHwnd);
		ShowWindow(mHwnd, nCmdShow);

		MSG msg = { 0, };
		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				gameProcessor->Update();
			}
		}

		gameProcessor->Release();

		return static_cast<int>(msg.wParam);
	}

	LRESULT CALLBACK MapToolWinApp::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		using namespace d2dFramework;

		//if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		//	return true;

		GameProcessor* gameProcessor = reinterpret_cast<GameProcessor*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

		switch (message)
		{
		case WM_CREATE:
		{
			placeInCenterOfScreen(hWnd, WS_OVERLAPPEDWINDOW, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		return 0;
	}

	void MapToolWinApp::placeInCenterOfScreen(HWND window, DWORD style, DWORD exStyle)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		RECT clientRect;
		GetClientRect(window, &clientRect);

		int clientWidth = clientRect.right - clientRect.left;
		int clientHeight = clientRect.bottom - clientRect.top;

		SetWindowPos(window, NULL,
			screenWidth / 2 - clientWidth / 2,
			screenHeight / 2 - clientHeight / 2,
			clientWidth, clientHeight, 0
		);
	}

	HWND MapToolWinApp::GetHwnd()
	{
		return mHwnd;
	}
	RECT MapToolWinApp::GetWindowRect()
	{
		return windowRect;
	}
}