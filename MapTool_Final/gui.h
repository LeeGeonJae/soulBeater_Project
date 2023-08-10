#pragma once
#include "defalut.h"
#include "SpriteRenderer.h"

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
	const float WIDTH = 500.f;
	const float HEIGHT = 500.f;

	static MapToolGui* mpInstance;

public:
	MapToolGui();
	~MapToolGui();

public:
	void GuiRender();

	void GridCreate();

	void ObjectTileSetting();

	void ObjectSetting();
	void ColliderSetting();
	void SpriteSetting();

	void ObjectImfomation();

public:
	bool GetIsCreateTileObject() {return mbIsCreateTileObjectButtonIstrue;}
	bool GetIsDeleteTileObject() {return mbIsDeleteTileObjectButtonIstrue;}
	bool GetIsCreateObject() {return mbIsCreateObjectButtonIstrue;}
	bool GetIsDeleteObject() {return mbIsDeleteObjectButtonIstrue;}
	bool GetIsCreateCollider() { return mbIsColliderCreate; }
	bool GetIsCreateSprite() { return mbIsSpriteCreate; }

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
	static std::wstring		TileBitmap;
	static int				TileSpriteWidth;
	static int				TileSpriteHeight;
	static int				TileColliderWidth;
	static int				TileColliderHeight;

	// 오브젝트별 Id값
	static IdSet mIdSetting;
	static int mPlayerId;
	static int mMonsterId;
	static int mItemId;
	static int mTileId;

	// 오브젝트 컴포넌트 세팅 값들
	static int ColliderWidth;
	static int ColliderHeight;

	// 스프라이트 컴포넌트 세팅 값들
	static d2dFramework::eSpriteType SpriteType;
	static int SpriteWidth;
	static int SpriteHeight;

	// 정보
	static int ItemImfoCurrentIndex;

private:
	bool mbIsCreateTileObjectButtonIstrue;
	bool mbIsDeleteTileObjectButtonIstrue;
	bool mbIsCreateObjectButtonIstrue;
	bool mbIsDeleteObjectButtonIstrue;

	bool mbIsColliderCreate;
	bool mbIsSpriteCreate;
};
