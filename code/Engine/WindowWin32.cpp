#include "stdafx.h"
#include "Window.h"
#if SE_PLATFORM_WINDOWS
#include "Application.h"
//-----------------------------------------------------------------------------
constexpr LPCWSTR WINDOW_CLASS_NAME = L"SapphireWindow";
//-----------------------------------------------------------------------------
#if SE_OPENGL
#	pragma comment(lib, "OpenGL32.lib")
struct GLWindowsWin32
{
	void Init(HWND &hwnd)
	{
		const PIXELFORMATDESCRIPTOR pfd = 
		{
			sizeof(PIXELFORMATDESCRIPTOR),1,
			PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
			PFD_TYPE_RGBA,32,
			0,0,0,0,0,0,
			0,0,0,
			0,0,0,0,
			24,0,0,
			PFD_MAIN_PLANE,0,0,0,0
		};
		hdc = GetDC(hwnd);
		int pixelFormat = ChoosePixelFormat(hdc, &pfd);
		SetPixelFormat(hdc, pixelFormat, &pfd);
		hrc = wglCreateContext(hdc);
		wglMakeCurrent(hdc, hrc);
	}

	void Close(HWND &hwnd)
	{
		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrc);
		ReleaseDC(hwnd, hdc);
	}

	HDC hdc;
	HGLRC hrc;
};
#endif
//-----------------------------------------------------------------------------
struct Window::WindowPlatform
{
	HINSTANCE Instance;
	HWND hwnd;
	GLWindowsWin32 glWnd;
};
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch ( msg )
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//-----------------------------------------------------------------------------
bool Window::init()
{
	m_pimpl = new Window::WindowPlatform;
	m_pimpl->Instance = GetModuleHandleW(nullptr);
	if ( !instance )
	{
		FatalError("Failed to get module handle");
		return false;
	}		

	WNDCLASS wndClass;
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_pimpl->Instance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	if ( !RegisterClass(&wndClass) )
	{
		FatalError("Failed to register window class");
		return false;
	}	

	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD styleEX = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	RECT winRect;
	winRect.left = (LONG)0;
	winRect.right = (LONG)m_config.width;
	winRect.top = (LONG)0;
	winRect.bottom = (LONG)m_config.height;
	DWORD screenLeft = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (m_config.width >> 1);
	DWORD screenTop = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (m_config.height >> 1);
	int screenHalfX = (int)GetSystemMetrics(SM_CXSCREEN) >> 1;
	int screenHalfY = (int)GetSystemMetrics(SM_CYSCREEN) >> 1;

	AdjustWindowRectEx(&winRect, style, false, styleEX);
	m_pimpl->hwnd = CreateWindowEx(styleEX, WINDOW_CLASS_NAME, TEXT("Game"), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style, screenLeft, screenTop, (winRect.right - winRect.left), (winRect.bottom - winRect.top), NULL, NULL, m_pimpl->Instance, NULL);
#if SE_OPENGL
	m_pimpl->glWnd.Init(m_pimpl->hwnd);
#endif
	ShowWindow(m_pimpl->hwnd, SW_SHOW);
	SetForegroundWindow(m_pimpl->hwnd);
	SetFocus(m_pimpl->hwnd);
	UpdateWindow(m_pimpl->hwnd);

	return true;
}
//-----------------------------------------------------------------------------
void Window::close()
{
	ShowCursor(true);
#if SE_OPENGL
	m_pimpl->glWnd.Close(m_pimpl->hwnd);
#endif
	DestroyWindow(m_pimpl->hwnd);
	UnregisterClass(WINDOW_CLASS_NAME, m_pimpl->Instance);
	SafeDelete(m_pimpl);
}
//-----------------------------------------------------------------------------
void Window::Update()
{
	MSG msg;
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		if ( msg.message == WM_QUIT )
		{
			Application::Exit();
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
//-----------------------------------------------------------------------------
void Window::Swap()
{
#if SE_OPENGL
	SwapBuffers(m_pimpl->glWnd.hdc);
#endif
}
//-----------------------------------------------------------------------------
#endif