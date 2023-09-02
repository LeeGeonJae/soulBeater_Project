#include "TestProjectJH.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	testProjectJH::TestProjectJH TestProjectJH(1920, 1080, L"demo");

	return d2dFramework::WinApp::Run(&TestProjectJH, hInstance, nCmdShow);
}
