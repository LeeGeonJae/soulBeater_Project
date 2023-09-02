#include "SoulBeaterProcessor.h"
#include "WinApp.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	TestGeon::SoulBeaterProcessor SoulBeaterProcessor(1920, 1080, L"demo");
	return d2dFramework::WinApp::Run(&SoulBeaterProcessor, hInstance, nCmdShow);
}
