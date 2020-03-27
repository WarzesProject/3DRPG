#include "stdafx.h"
#include "Game.h"
#if SE_PLATFORM_WINDOWS
//-----------------------------------------------------------------------------
#pragma comment(lib, "3rdparty.lib")
#pragma comment(lib, "Engine.lib")
#pragma comment(lib, "ImportCode.lib")
#pragma comment(lib, "Winmm.lib")
//-----------------------------------------------------------------------------
int WINAPI wWinMain(
	[[maybe_unused]] _In_ HINSTANCE hInstance,
	[[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance,
	[[maybe_unused]] _In_ LPWSTR lpCmdLine,
	[[maybe_unused]] _In_ int nCmdShow )
{
	return Game::Main();
}
//-----------------------------------------------------------------------------
#endif