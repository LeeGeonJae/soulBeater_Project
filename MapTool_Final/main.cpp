#include "defalut.h"


#include "SoulBeaterProcessor.h"
#include "MapToolWinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	mapTool::SoulBeaterProcessor soulBeaterProcessor(1920, 1080, L"demo");
	return mapTool::MapToolWinApp::Run(&soulBeaterProcessor, hInstance, nCmdShow);
}