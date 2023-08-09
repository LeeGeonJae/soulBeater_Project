#pragma once
#include "defalut.h"
#include "ResourceManager.h"
#include "KeyManager.h"

class Mesh;
class Texture;
class Example;

class MapToolProcess
{
public:
	MapToolProcess();
	~MapToolProcess();

public:
	void Initialize(HWND _hwnd);
	void Loop();
	void Update();
	void Render();
	void ImGuiRender();

public:
	vector<Mesh*>	m_Mesh;
	vector<Mesh*>	m_SelectedMesh;
	Example*	m_Example;

	HWND m_hwnd;
	ResourceManager* m_ResourceManager = new ResourceManager;
	KeyManager* m_KeyManager = new KeyManager;
};