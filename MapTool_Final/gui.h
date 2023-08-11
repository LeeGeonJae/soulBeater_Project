#pragma once
#include "defalut.h"
#include "SpriteRenderer.h"
#include "eObjectType.h"

#include <d3d9.h>
#include <utility>

class RenderManager;

enum class IdSet
{
	DEFALUE = 0,
	PLAYERID,
	MONSTERID,
	ITEMID,
	END
};

// const
const int PLAYER_ID_START = 10000;
const int MONSTER_ID_START = 20000;
const int ITEM_ID_START = 30000;
const int TILE_ID_START = 40000;

class MapToolGui
{
public:
	const float WIDTH = 600.f;
	const float HEIGHT = 1200.f;

	static MapToolGui* mpInstance;

public:
	MapToolGui();
	~MapToolGui();

public:
	void GuiRender();
	
	void GuiMenu();

	void GridCreate();

	void ObjectTileSetting();

	void ObjectSetting();
	void ObjectSpriteSetting();

	void ComponentEdit();
	void ColliderSetting();
	void SpriteSetting();

	void ObjectImfomation();
	void TileObjectImfomation();

	void ImGuiImageLoading();

public:
	bool GetIsCreateTileObject() {return mbIsCreateTileObjectButtonIstrue;}
	bool GetIsCreateTileCollider() {return mbIsCreateTileColliderIstrue;}
	bool GetIsDeleteTileObject() {return mbIsDeleteTileObjectButtonIstrue;}
	bool GetIsCreateObject() {return mbIsCreateObjectButtonIstrue;}
	bool GetIsDeleteObject() {return mbIsDeleteObjectButtonIstrue;}
	bool GetIsCreateCollider() { return mbIsColliderCreate; }
	bool GetIsCreateSprite() { return mbIsSpriteCreate; }
	bool GetIsImageLoading() { return mbIsImageLoading; }
	bool GetIsImageSeleted() { return mbIsImageSelected; }
	bool GetIsCreateScene() { return mbIsCreateScene; }
	bool GetIsSave() { return mbIsSaveButton; }
	bool GetIsLoad() { return mbIsLoadButton; }

public:
	// handle window creation & destruction
	void CreateHWindow(const char* windowName, const char* className);
	void DestroyHWindow();

	// handle device creation & destruction
	bool CreateDevice();
	void ResetDevice();
	void DestroyDevice();

	// handle ImGui creation & destruction
	void CreateImGui();
	void DestroyImGui();

	void BeginRender();
	void EndRender();
	void Render();

public:
	HWND GetHwnd() { return mHWnd; }
	WNDCLASSEXA GetWindowClass() { return mWindowClass; }
	POINTS GetGuiPosition() { return mPosition; }
	PDIRECT3D9 GetD3D() { return md3d; }
	LPDIRECT3DDEVICE9 GetDevice() { return mDevice; }
	D3DPRESENT_PARAMETERS GetPresentParameter() { return mPresentParameters; }

	void SetClickPoint(LPARAM clickpoint) { mPosition = MAKEPOINTS(clickpoint); }
	void SetBackBufferWidth(WORD lower, WORD high) 
	{ 
		mPresentParameters.BackBufferWidth = lower; 
		mPresentParameters.BackBufferWidth = high;
	}

private:
	// constant window size
	bool exit;

	bool bOnbutton;

	// winapi window vars
	HWND mHWnd;
	WNDCLASSEXA mWindowClass;

	// points for window movement
	POINTS mPosition;

	// direct x state vars
	PDIRECT3D9 md3d;
	LPDIRECT3DDEVICE9 mDevice;
	D3DPRESENT_PARAMETERS mPresentParameters;

public:
	// 씬 맵 ID
	static int SceneId;

	// create 할 시 격자판과 격자판을 다룰 2d 배열 생성
	static unsigned int mWidth;
	static unsigned int mHeight;
	static unsigned int mGridDistance;

	static std::vector<std::vector<bool>>		mbIsChecked;		// 이거 루프 돌면서 클릭 처리
	static std::vector<std::vector<bool>>		mbIsObject;			// 여기는 해당 격자칸에 오브젝트가 존재하는지
	static std::map<std::pair<int, int>, int>	mObjectIdMap;		// 해당 칸에 오브젝트가 있는지
	static std::queue<int>						mObjectSequence;	// 선택된 오브젝트 순서

	// Tile 오브젝트
	static std::vector<std::vector<bool>>		mbIsTileObject;		// 여기는 해당 격자칸에 타일 오브젝트가 존재하는지
	static std::map<std::pair<int, int>, int>	mTileObjectIdMap;	// 해당 칸에 타일 오브젝트가 있는지
	static int				TileSpriteWidth;
	static int				TileSpriteHeight;
	static int				TileColliderWidth;
	static int				TileColliderHeight;
	static int				TileObjectItemCurrentIndex;

	// 오브젝트별 Id값
	static IdSet mIdSetting;
	static int mPlayerId;
	static int mMonsterId;
	static int mItemId;
	static int mTileId;
	static d2dFramework::eObjectType ObjectType;

	// 오브젝트 컴포넌트 세팅 값들
	static bool bIsColliderCheck;
	static int ColliderWidth;
	static int ColliderHeight;

	// 스프라이트 컴포넌트 세팅 값들
	static bool bIsSpriteCheck;
	static d2dFramework::eSpriteType SpriteType;
	static int	CreateSpriteWidth;
	static int	CreateSpriteHeight;
	static int	SpriteWidth;
	static int	SpriteHeight;
	static int	ObjectItemCurrentIndex;

	// 정보
	static int ItemImfoCurrentIndex;

	// ImGui의 리스트 이름(함수와 호환이 될 자료형 리스트 이름)
	static std::vector<std::string> ImageListName;
	static std::vector<std::wstring> WstringImageListName;
	static std::wstring wstrImageName;
	static std::wstring wstrImagePath;
	// ImGui 텍스트 박스에 입력될 static char
	static char ImageName[100];
	static char ImagePath[256];
	// 이미지 리스트 생성 및 이미지 클릭 시 이미지 적용
	static int ItemImageCurrentIndex;

private:
	bool mbIsCreateTileObjectButtonIstrue;
	bool mbIsCreateTileColliderIstrue;
	bool mbIsDeleteTileObjectButtonIstrue;
	bool mbIsCreateObjectButtonIstrue;
	bool mbIsDeleteObjectButtonIstrue;

	bool mbIsColliderCreate;
	bool mbIsSpriteCreate;

	bool mbIsImageLoading;
	bool mbIsImageSelected;

	bool mbIsCreateScene;
	bool mbIsSaveButton;
	bool mbIsLoadButton;
};
