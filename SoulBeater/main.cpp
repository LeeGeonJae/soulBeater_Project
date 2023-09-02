#include "SoulBeaterProcessor.h"
#include "WinApp.h"
#include "BaseEntity.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <unordered_set>

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	int result = 0;
	// 여기서 발생한 메모리 누수는 static 맴버를 의미한다.
	_CrtDumpMemoryLeaks();
	{
		soulBeater::SoulBeaterProcessor soulBeaterProcessor(1920, 1080, L"demo");
		result = d2dFramework::WinApp::Run(&soulBeaterProcessor, hInstance, nCmdShow);
	}
	// static IDSet만큼 메모리 누수 발생, 실 메모리 누수는 아님 -> 1024, 16, 24, 16
	_CrtDumpMemoryLeaks();

 	return result;
}
