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
	// create �� �� �����ǰ� �������� �ٷ� 2d �迭 ����
	static unsigned int mWidth;
	static unsigned int mHeight;
	static unsigned int mGridDistance;

	static std::vector<std::vector<bool>>		mbIsChecked;		// �̰� ���� ���鼭 Ŭ�� ó��
	static std::vector<std::vector<bool>>		mbIsObject;			// ����� �ش� ����ĭ�� ������Ʈ�� �����ϴ���
	static std::map<std::pair<int, int>, int>	mObjectIdMap;		// �ش� ĭ�� ������Ʈ�� �ִ���
	static std::queue<int>						mObjectSequence;	// ���õ� ������Ʈ ����

	// Tile ������Ʈ
	static std::vector<std::vector<bool>>		mbIsTileObject;		// ����� �ش� ����ĭ�� Ÿ�� ������Ʈ�� �����ϴ���
	static std::map<std::pair<int, int>, int>	mTileObjectIdMap;	// �ش� ĭ�� Ÿ�� ������Ʈ�� �ִ���
	static std::wstring		TileBitmap;
	static int				TileSpriteWidth;
	static int				TileSpriteHeight;
	static int				TileColliderWidth;
	static int				TileColliderHeight;

	// ������Ʈ�� Id��
	static IdSet mIdSetting;
	static int mPlayerId;
	static int mMonsterId;
	static int mItemId;
	static int mTileId;

	// ������Ʈ ������Ʈ ���� ����
	static int ColliderWidth;
	static int ColliderHeight;

	// ��������Ʈ ������Ʈ ���� ����
	static d2dFramework::eSpriteType SpriteType;
	static int SpriteWidth;
	static int SpriteHeight;

	// ����
	static int ItemImfoCurrentIndex;

private:
	bool mbIsCreateTileObjectButtonIstrue;
	bool mbIsDeleteTileObjectButtonIstrue;
	bool mbIsCreateObjectButtonIstrue;
	bool mbIsDeleteObjectButtonIstrue;

	bool mbIsColliderCreate;
	bool mbIsSpriteCreate;
};
