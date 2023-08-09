#pragma once

#include <windows.h>
#include <memory>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <filesystem>
#include <map>

#include "imgui.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>

#include <DirectXTex.h>
#include <DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectXTex.lib")
#endif

#include "Data.h"

using namespace std;

const int WIDTH = 1500;
const int HEIGHT = 900;
const int CANVAS_WIDTH = WIDTH / 80;
const int CANVAS_HEIGHT = HEIGHT / 80;

// 수학의 4차원 벡터(vector)를 의미합니다. 
// std::vector와는 다릅니다.
struct Vec4
{
	float r;
	float g;
	float b;
	float a;
};

struct RECT_F
{
	float left;
	float right;
	float top;
	float bottom;
};

struct Vec2
{
	float x;
	float y;
};

struct Vertex
{
	Vec4 pos;
	Vec4 color;
	Vec2 uv;
};

struct Transform
{
	Vec4 offset;
};