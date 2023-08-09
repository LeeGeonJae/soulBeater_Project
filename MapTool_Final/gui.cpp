#include "gui.h"

#include "../Bin/include/imgui/imgui.h"
#include "../Bin/include/imgui/imgui_impl_dx9.h"
#include "../Bin/include/imgui/imgui_impl_win32.h"

#pragma comment(lib, "D3d9.lib")


#include "MapToolWinApp.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "SoulBeaterProcessor.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter);

// 변수 초기화
MapToolGui* MapToolGui::mpInstance = nullptr;

MapToolGui::MapToolGui()
	:bOnbutton(false)
	, exit(true)
	, mHWnd(nullptr)
	, mWindowClass{}
	, mPosition{}
	, md3d(nullptr)
	, mDevice(nullptr)
	, mPresentParameters{}
	, CreateObjectButtonIstrue(false)
{
	mpInstance = this;
}

MapToolGui::~MapToolGui()
{
}

void MapToolGui::Render()
{
	using namespace d2dFramework;

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Map Tool Menu",
		&exit,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove
	);

	GuiRender();

	ImGui::End();
}

void MapToolGui::GuiRender()
{
	// 문자열{ 이미지 이름, 파일 경로 } 을 입력받아 비트맵 로딩하여 이미지 이름으로 리스트에 추가하기 
	//if (ImGui::TreeNode("Image Create"))
	//	ImGuiImageLoading();

	// 격자판 생성 시
	if (ImGui::TreeNode("Grid Setting & Create"))
		GridCreate();

	if (ImGui::TreeNode("Object Setting"))
		ObjectButton();
}

void MapToolGui::GridCreate()
{
	static int GridArea[2] = {};
	static int GridInstance = 0;

	// 그리드 가로,세로 값세팅 및 그리드 사이 간격 길이 입력
	ImGui::InputInt2("Grid Area Width, Height", GridArea);
	ImGui::InputInt("Grid Distance", &GridInstance);

	// 그리드 세팅 버튼을 누르면 그리드 칸에 맞춰서 세팅
	if (ImGui::Button("Grid Setting"))
	{
		for (int i = 0; i < mbIsChecked.size(); i++)
			mbIsChecked[i].clear();
		mbIsChecked.clear();

		for (int i = 0; i < mbIsObject.size(); i++)
			mbIsObject[i].clear();
		mbIsObject.clear();

		mWidth = GridArea[0];
		mHeight = GridArea[1];
		mGridDistance = GridInstance;

		// 그리드에 맞게 리사이즈
		if (mHeight != 0 && mGridDistance != 0 && mWidth != 0 && mGridDistance != 0)
		{
			mbIsChecked.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
			mbIsObject.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
		}
		else
		{
			mbIsChecked.clear();
			mbIsObject.clear();
		}

		// 모든 오브젝트 삭제
		for (auto e : mObjectIdMap)
			d2dFramework::ObjectManager::GetInstance()->DeletObject(e.second);
		mObjectIdMap.clear();
	}

	ImGui::TreePop();
}

void MapToolGui::ObjectButton()
{
	if (ImGui::Button("CreateObject"))
		CreateObjectButtonIstrue = true;
	else
		CreateObjectButtonIstrue = false;

	if (ImGui::Button("DeleteObject"))
		DeleteObjectButtonIstrue = true;
	else
		DeleteObjectButtonIstrue = false;

	ImGui::TreePop();
}

// handle window creation & destruction
void MapToolGui::CreateHWindow(const char* windowName, const char* className)
{
	mWindowClass.cbSize = sizeof(WNDCLASSEXA);
	mWindowClass.style = CS_CLASSDC;
	mWindowClass.lpfnWndProc = WindowProcess;
	mWindowClass.cbClsExtra = 0;
	mWindowClass.cbWndExtra = 0;
	mWindowClass.hInstance = GetModuleHandleA(0);
	mWindowClass.hIcon = 0;
	mWindowClass.hCursor = 0;
	mWindowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	mWindowClass.lpszMenuName = 0;
	mWindowClass.lpszClassName = "Gui";
	mWindowClass.hIconSm = 0;

	RegisterClassExA(&mWindowClass);

	mHWnd = CreateWindowW(
		L"Gui",
		L"ImGui",
		WS_POPUP,
		2700,
		200,
		WIDTH,
		HEIGHT,
		0,
		0,
		mWindowClass.hInstance,
		0
	);

	ShowWindow(mHWnd, SW_SHOWDEFAULT);
	UpdateWindow(mHWnd);
}

void MapToolGui::DestroyHWindow()
{
	DestroyWindow(mHWnd);
	UnregisterClass(L"Gui", mWindowClass.hInstance);
}

// handle device creation & destruction
bool MapToolGui::CreateDevice()
{
	md3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!md3d)
		return false;

	ZeroMemory(&mPresentParameters, sizeof(mPresentParameters));

	mPresentParameters.Windowed = TRUE;
	mPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	mPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	mPresentParameters.EnableAutoDepthStencil = TRUE;
	mPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;
	mPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;

	// 디스플레이 어댑터를 나타내는 디바이스를 만드는 함수입니다.
	if (md3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		mHWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&mPresentParameters,
		&mDevice) < 0)
		return false;

	return true;
}

void MapToolGui::ResetDevice()
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = mDevice->Reset(&mPresentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void MapToolGui::DestroyDevice()
{
	if (mDevice)
	{
		mDevice->Release();
		mDevice = nullptr;
	}

	if (md3d)
	{
		md3d->Release();
		md3d = nullptr;
	}
}

// handle ImGui creation & destruction
void MapToolGui::CreateImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(mHWnd);
	ImGui_ImplDX9_Init(mDevice);
}

void MapToolGui::DestroyImGui()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MapToolGui::BeginRender()
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	// Start ther Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void MapToolGui::EndRender()
{
	ImGui::EndFrame();

	mDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	mDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	mDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	mDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (mDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		mDevice->EndScene();
	}

	const auto result = mDevice->Present(0, 0, 0, 0);

	// Handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && mDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

LRESULT __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter)
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

	switch (message)
	{
	case WM_SIZE:
	{
		if (MapToolGui::mpInstance->GetDevice() && wideParameter != SIZE_MINIMIZED)
		{
			MapToolGui::mpInstance->SetBackBufferWidth(LOWORD(longParameter), HIWORD(longParameter));
			MapToolGui::mpInstance->ResetDevice();
		}
	}
	return 0;

	case WM_SYSCOMMAND:
	{
		if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
	}
	break;

	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	return 0;

	case WM_LBUTTONDOWN:
	{
		//MapToolGui::mpInstance->GetGuiPosition() = MAKEPOINTS(longParameter);	// set click points;
		MapToolGui::mpInstance->SetClickPoint(longParameter);	// set click points;
	}
	return 0;

	case WM_MOUSEMOVE:
	{
		// 창이동?
		if (wideParameter == MK_LBUTTON)
		{
			const auto points = MAKEPOINTS(longParameter);
			auto rect = ::RECT{};

			GetWindowRect(MapToolGui::mpInstance->GetHwnd(), &rect);

			rect.left += points.x - MapToolGui::mpInstance->GetGuiPosition().x;
			rect.top += points.y - MapToolGui::mpInstance->GetGuiPosition().y;

			if (MapToolGui::mpInstance->GetGuiPosition().x >= 0 &&
				MapToolGui::mpInstance->GetGuiPosition().x <= MapToolGui::mpInstance->WIDTH &&
				MapToolGui::mpInstance->GetGuiPosition().y >= 0 && MapToolGui::mpInstance->GetGuiPosition().y <= 19)
				SetWindowPos(
					MapToolGui::mpInstance->GetHwnd(),
					HWND_TOPMOST,
					rect.left,
					rect.top,
					0, 0,
					SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
				);
		}
	}
	}

	return DefWindowProcW(window, message, wideParameter, longParameter);
}