#include "Example.h"
#include "Mesh.h"
#include "Texture.h"
#include "MapToolProcess.h"

void Init();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// �ܼ�â�� ������ ����뿡 �����մϴ�.
int main()
{
	// ������ ����
	Init();


	// ���� ���μ���
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

	// ������ �׷����� �ػ󵵸� �����ϱ� ����
	RECT wr = { 0, 0, WIDTH, HEIGHT };    // set the size, but not the position
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // adjust the size

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"HongLabGraphics Example",
		WS_OVERLAPPEDWINDOW,
		100, // ������ ���� ����� x ��ǥ
		100, // ������ ���� ����� y ��ǥ
		wr.right - wr.left, // ������ ���� ���� �ػ�
		wr.bottom - wr.top, // ������ ���� ���� �ػ�
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
