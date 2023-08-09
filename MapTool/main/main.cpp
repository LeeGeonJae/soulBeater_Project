#include "Example.h"
#include "Mesh.h"
#include "Texture.h"
#include "MapToolProcess.h"

void Init();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 콘솔창이 있으면 디버깅에 유리합니다.
int main()
{
	// 윈도우 세팅
	Init();


	// 맵툴 프로세스
	MapToolProcess process;
	process.Initialize(Data::g_hwnd);
	process.Loop();

	//UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

void Init()
{
	WNDCLASSEX wc = {
	sizeof(WNDCLASSEX),
	CS_CLASSDC,
	WndProc,
	0L,
	0L,
	GetModuleHandle(NULL),
	NULL,
	NULL,
	NULL,
	NULL,
	L"HongLabGraphics", // lpszClassName, L-string
	NULL
	};

	RegisterClassEx(&wc);

	// 실제로 그려지는 해상도를 설정하기 위해
	RECT wr = { 0, 0, WIDTH, HEIGHT };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"HongLabGraphics Example",
		WS_OVERLAPPEDWINDOW,
		100, // 윈도우 좌측 상단의 x 좌표
		100, // 윈도우 좌측 상단의 y 좌표
		wr.right - wr.left, // 윈도우 가로 방향 해상도
		wr.bottom - wr.top, // 윈도우 세로 방향 해상도
		NULL,
		NULL,
		wc.hInstance,
		NULL);

	Data::g_hwnd = hwnd;
	Data::g_hdc = GetDC(Data::g_hwnd);

	ShowWindow(Data::g_hwnd, SW_SHOWDEFAULT);
	UpdateWindow(Data::g_hwnd);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		// Reset and resize swapchain
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu 
			return 0;
		break;
	case WM_LBUTTONUP:
		std::cout << (float)Data::MousePos.x << std::endl;
		std::cout << (float)Data::MousePos.y << std::endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
