#pragma once
#include "defalut.h"

struct Point
{
	float x;
	float y;
};

namespace Data
{
	extern HWND g_hwnd;
	extern HDC g_hdc;
	extern HBRUSH hbrushRed;
	extern HBRUSH hbrushBlue;
	extern HPEN hBlackPen;
	extern HPEN hRedPen;
	extern HPEN hGreenPen;

	extern int Grid[2];
	extern Point MousePos;

}