#pragma once
#include "defalut.h"

#include <d3d9.h>
#include <utility>

class RenderManager;

class MapToolGui
{
public:
	const float WIDTH = 500.f;
	const float HEIGHT = 300.f;

	static MapToolGui* mpInstance;

public:
	MapToolGui();
	~MapToolGui();

public:
	// handle window creation & destruction
	void CreateHWindow(const char* windowName, const char* className) ;
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
	void GuiRender();

	void GridCreate();
	void ObjectButton();

public:
	unsigned int GetGridWidth() { return mWidth; }
	unsigned int GetGridHeight() { return mHeight; }
	unsigned int GetGridDistance() { return mGridDistance; }
	bool GetIsCreateObject() {return CreateObjectButtonIstrue;}
	bool GetIsDeleteObject() {return DeleteObjectButtonIstrue;}

	std::vector<std::vector<bool>> GetIsChecked() { return mbIsChecked; }
	std::vector<std::vector<bool>> GetIsObject() { return mbIsObject; }
	std::map<std::pair<int, int>, int> GetObjectId() { return mObjectIdMap; }

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
	void SetIsChecked(int y, int x, bool b) { mbIsChecked[y][x] = b; }
	void SetIsObject(int y, int x, bool b) { mbIsObject[y][x] = b; }
	void SetObjectMapInsert(int x, int y, int id) { mObjectIdMap.insert(std::make_pair(std::make_pair(x, y), id)); }

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
	unsigned int mWidth;
	unsigned int mHeight;
	unsigned int mGridDistance;

	std::vector<std::vector<bool>>		mbIsChecked; // 이거 루프 돌면서 클릭 처리
	std::vector<std::vector<bool>>		mbIsObject; // 여기는 해당 격자칸에 오브젝트가 존재하는지
	std::map<std::pair<int, int>, int>	mObjectIdMap;

private:
	bool CreateObjectButtonIstrue;
	bool DeleteObjectButtonIstrue;
};
