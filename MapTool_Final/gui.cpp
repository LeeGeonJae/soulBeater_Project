#include "gui.h"

#include "../Bin/include/imgui/imgui.h"
#include "../Bin/include/imgui/imgui_impl_dx9.h"
#include "../Bin/include/imgui/imgui_impl_win32.h"

#pragma comment(lib, "D3d9.lib")

#include "MapToolWinApp.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "AABBCollider.h"
#include "SpriteRenderer.h"
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


// static 변수 초기화
MapToolGui* MapToolGui::mpInstance = nullptr;

unsigned int MapToolGui::mWidth = 0;
unsigned int MapToolGui::mHeight = 0;
unsigned int MapToolGui::mGridDistance = 0;

// 오브젝트 체크, 오브젝트 ID, 오브젝트 시퀀스 ( 미완성 )
std::vector<std::vector<bool>>		MapToolGui::mbIsChecked = {};
std::vector<std::vector<bool>>		MapToolGui::mbIsObject = {};
std::map<std::pair<int, int>, int>	MapToolGui::mObjectIdMap = {};
std::queue<int>						MapToolGui::mObjectSequence = {};

// 타일 오브젝트
std::vector<std::vector<bool>>		MapToolGui::mbIsTileObject = {};
std::map<std::pair<int, int>, int>	MapToolGui::mTileObjectIdMap = {};
std::wstring	MapToolGui::TileBitmap = L"Golem";
int				MapToolGui::TileSpriteWidth = 0;
int				MapToolGui::TileSpriteHeight = 0;
int				MapToolGui::TileColliderWidth = 0;
int				MapToolGui::TileColliderHeight = 0;

// Id 세팅
IdSet MapToolGui::mIdSetting = IdSet::DEFALUE;
int MapToolGui::mPlayerId = PLAYER_ID_START;
int MapToolGui::mMonsterId = MONSTER_ID_START;
int MapToolGui::mItemId = ITEM_ID_START;
int MapToolGui::mTileId = TILE_ID_START;

// 오브젝트 콜라이더 사이즈
int MapToolGui::ColliderWidth = 0;
int MapToolGui::ColliderHeight = 0;

// 스프라이트 세팅 값들
d2dFramework::eSpriteType MapToolGui::SpriteType = d2dFramework::eSpriteType::Circle;
int MapToolGui::SpriteWidth = 0;
int MapToolGui::SpriteHeight = 0;

int MapToolGui::ItemImfoCurrentIndex = 0;

MapToolGui::MapToolGui()
	:bOnbutton(false)
	, exit(true)
	, mHWnd(nullptr)
	, mWindowClass{}
	, mPosition{}
	, md3d(nullptr)
	, mDevice(nullptr)
	, mPresentParameters{}
	, mbIsCreateTileObjectButtonIstrue(false)
	, mbIsDeleteTileObjectButtonIstrue(false)
	, mbIsCreateObjectButtonIstrue(false)
	, mbIsDeleteObjectButtonIstrue(false)
	, mbIsColliderCreate(false)
	, mbIsSpriteCreate(false)
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

	if (ImGui::TreeNode("Tile Setting"))
		ObjectTileSetting();

	if (ImGui::TreeNode("Object Setting"))
		ObjectSetting();

	if (ImGui::TreeNode("Object Impomation"))
		ObjectImfomation();
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

		for (int i = 0; i < mbIsTileObject.size(); i++)
			mbIsTileObject[i].clear();
		mbIsTileObject.clear();

		mWidth = GridArea[0];
		mHeight = GridArea[1];
		mGridDistance = GridInstance;

		// 그리드에 맞게 리사이즈
		if (mHeight != 0 && mGridDistance != 0 && mWidth != 0 && mGridDistance != 0)
		{
			mbIsChecked.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
			mbIsObject.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
			mbIsTileObject.resize(mHeight / mGridDistance, std::vector<bool>(mWidth / mGridDistance, false));
		}
		else
		{
			mbIsChecked.clear();
			mbIsObject.clear();
			mbIsTileObject.clear();

			while(!mObjectSequence.empty())
				mObjectSequence.pop();
		}

		// 모든 오브젝트 삭제
		for (auto e : mObjectIdMap)
			d2dFramework::ObjectManager::GetInstance()->DeletObject(e.second);
		mObjectIdMap.clear();
		for (auto e : mTileObjectIdMap)
			d2dFramework::ObjectManager::GetInstance()->DeletObject(e.second);
		mTileObjectIdMap.clear();
	}

	ImGui::TreePop();
}

// 오브젝트 생성 및 삭제 및 컴포넌트 세팅
void MapToolGui::ObjectSetting()
{
	static int ObjectItemCurrentIndex = 0;
	static std::string ListName[] = { "Defalut", "PlayerId", "MonsterId", "ItemId" };

	if (ImGui::BeginListBox("Id Setting"))
	{
		for (int n = 0; n < (int)IdSet::END; n++)
		{
			const bool Selected = (ObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ObjectItemCurrentIndex = n;

				switch (ObjectItemCurrentIndex)
				{
				case 0:
					mIdSetting = IdSet::DEFALUE;
					break;
				case 1:
					mIdSetting = IdSet::PLAYERID;
					break;
				case 2:
					mIdSetting = IdSet::MONSTERID;
					break;
				case 3:
					mIdSetting = IdSet::ITEMID;
					break;
				}
			}
		}

		ImGui::EndListBox();
	}

	std::string idtext = "Seleted Id : " + ListName[ObjectItemCurrentIndex];
	ImGui::Text(idtext.c_str());

	// 오브젝트 생성 버튼
	if (ImGui::Button("CreateObject"))
		mbIsCreateObjectButtonIstrue = true;
	else
		mbIsCreateObjectButtonIstrue = false;

	// 오브젝트 삭제 버튼
	if (ImGui::Button("DeleteObject"))
		mbIsDeleteObjectButtonIstrue = true;
	else
		mbIsDeleteObjectButtonIstrue = false;

	// 콜라이더 컴포넌트 세팅
	if (ImGui::TreeNode("Collider Setting"))
		ColliderSetting();

	// 스프라이트 컴포넌트 세팅
	if (ImGui::TreeNode("Sprite Setting"))
		SpriteSetting();

	ImGui::TreePop();
}

void MapToolGui::ObjectTileSetting()
{
	static int ObjectItemCurrentIndex = 0;
	static std::vector<std::string> ListName{ "Golem", "Charactor" };

	ImGui::InputInt("Sprite Width", &TileSpriteWidth);
	ImGui::InputInt("Sprite Height", &TileSpriteHeight);

	// 타일 Bitmap 선택
	if (ImGui::BeginListBox("Id Setting"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ObjectItemCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ObjectItemCurrentIndex = n;

				if (ObjectItemCurrentIndex == 0)
					TileBitmap = L"Golem";
				else if (ObjectItemCurrentIndex == 1)
					TileBitmap = L"Charactor";
			}
		}

		ImGui::EndListBox();
	}

	// 오브젝트 생성 버튼
	if (ImGui::Button("CreateObject"))
		mbIsCreateTileObjectButtonIstrue = true;
	else
		mbIsCreateTileObjectButtonIstrue = false;

	// 오브젝트 삭제 버튼
	if (ImGui::Button("DeleteObject"))
		mbIsDeleteTileObjectButtonIstrue = true;
	else
		mbIsDeleteTileObjectButtonIstrue = false;

	ImGui::TreePop();
}

// 콜라이더 세팅
void MapToolGui::ColliderSetting()
{
	ImGui::InputInt("Collider Width", &ColliderWidth);
	ImGui::InputInt("Collider Hight", &ColliderHeight);

	if (ImGui::Button("Collider Create & Setting"))
		mbIsColliderCreate = true;
	else
		mbIsColliderCreate = false;

	ImGui::TreePop();
}

void MapToolGui::SpriteSetting()
{
	static int ItemSpriteCurrentIndex = 0;
	static std::vector<std::string> ListName = { "Rectangle", "Circle", "Sprite" };

	if (ImGui::BeginListBox("SpriteType List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemSpriteCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemSpriteCurrentIndex = n;

				switch (ItemSpriteCurrentIndex)
				{
				case 0:
					SpriteType = d2dFramework::eSpriteType::Rectangle;
					break;
				case 1:
					SpriteType = d2dFramework::eSpriteType::Circle;
					break;
				case 2:
					SpriteType = d2dFramework::eSpriteType::Sprite;
					break;
				}
			}
		}
		ImGui::EndListBox();
	}

	std::string seletedspritetype = "Selected : " + ListName[ItemSpriteCurrentIndex];
	ImGui::Text(seletedspritetype.c_str());

	ImGui::InputInt("Sprite Width", &SpriteWidth);
	ImGui::InputInt("Sprite Height", &SpriteHeight);

	if (ImGui::Button("Sprite Create"))
		mbIsSpriteCreate = true;
	else
		mbIsSpriteCreate = false;

	ImGui::TreePop();
}

void MapToolGui::ObjectImfomation()
{
	using namespace d2dFramework;

	static std::vector<std::string> ListName;
	static int ObjectListCount = 0;
	std::vector<int> ListId;

	ListName.clear();
	ListId.clear();
	ObjectListCount = 0;

	for (int i = 0; i < mbIsChecked.size(); i++)
	{
		for (int j = 0; j < mbIsChecked[i].size(); j++)
		{
			if (mbIsChecked[i][j] == true && mbIsObject[i][j] == true)
			{
				std::pair<int, int> key = std::make_pair(i, j);
				auto iter = mObjectIdMap.find(key);

				std::string idlist;

				if (iter->second >= ITEM_ID_START)
				{
					idlist = "Item Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}
				else if (iter->second >= MONSTER_ID_START)
				{
					idlist = "Monster Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}
				else if (iter->second >= PLAYER_ID_START)
				{
					idlist = "Player Object " + std::to_string(ObjectListCount);
					ObjectListCount++;
				}

				ListName.push_back(idlist);
				ListId.push_back(iter->second);
			}
		}
	}

	if (ImGui::BeginListBox("Image List"))
	{
		for (int n = 0; n < ListName.size(); n++)
		{
			const bool Selected = (ItemImfoCurrentIndex == n);
			if (ImGui::Selectable(ListName[n].c_str(), Selected))
			{
				ItemImfoCurrentIndex = n;
			}
		}
		ImGui::EndListBox();
	}
	
	if (ListId.size() != 0)
	{
		GameObject* object = ObjectManager::GetInstance()->FindObjectOrNull(ListId[ItemImfoCurrentIndex]);
		Transform* TransformComponent = object->GetComponent<Transform>();
		SpriteRenderer* SpriteComponent = object->GetComponent<SpriteRenderer>();
		AABBCollider* ColliderComponent = object->GetComponent<AABBCollider>();

		std::string text = ListName[ItemImfoCurrentIndex] + ", Id : " + std::to_string(ListId[ItemImfoCurrentIndex]);
		ImGui::Text(text.c_str());
		text.clear();

		// 트랜스폼 위치 값 출력
		if (TransformComponent != nullptr)
		{
			ImGui::Text("Transform Is Valid");
			text = "Location X : " + std::to_string(TransformComponent->GetTransform().dx);
			ImGui::Text(text.c_str());
			text.clear();
			text = "Location Y : " + std::to_string(TransformComponent->GetTransform().dy);
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("Transform Is not Valid");

		// 스프라이트 정보 출력
		if (SpriteComponent != nullptr)
		{
			ImGui::Text("SpriteComponent Is Valid");
			if (SpriteComponent->GetBitmap() != nullptr)
				ImGui::Text("Bitmap Is Valid");
			else
				ImGui::Text("Bitmap Is not Valid");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Rectangle)
				ImGui::Text("SpriteType Is Rectangle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Circle)
				ImGui::Text("SpriteType Is Circle");
			if (SpriteComponent->GetSpriteType() == eSpriteType::Sprite)
				ImGui::Text("SpriteType Is Sprite");
			text = "Size X : " + std::to_string(SpriteComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size Y : " + std::to_string(SpriteComponent->GetSize().GetY());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("SpriteComponnet Is not Valid");

		// 콜라이더 정보 출력 ( AABB ) 
		if (ColliderComponent != nullptr)
		{
			ImGui::Text("ColliderComponent Is Valid");
			ImGui::Text("ColliderComponent Type Is AABBCollider");
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
			text = "Size X : " + std::to_string(ColliderComponent->GetSize().GetX());
			ImGui::Text(text.c_str());
			text.clear();
		}
		else
			ImGui::Text("ColliderComponent Is not Valid");
	}

	ImGui::TreePop();
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
		1500,
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
