#include "Data.h"

int Data::Grid[2] = {};
Point Data::MousePos = {};
HWND Data::g_hwnd = nullptr;
HDC Data::g_hdc = nullptr;

HBRUSH Data::hbrushRed = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH Data::hbrushBlue = CreateSolidBrush(RGB(0, 255, 0));
HPEN Data::hBlackPen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
HPEN Data::hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
HPEN Data::hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));