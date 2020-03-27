#include "stdafx.h"
#include "Window.h"
#if SE_PLATFORM_WINDOWS
#include "Application.h"
//-----------------------------------------------------------------------------
constexpr LPCWSTR WINDOW_CLASS_NAME = L"SapphireWindow";
//-----------------------------------------------------------------------------
struct Window::WindowPlatform
{
	~WindowPlatform()
	{
		DestroyWindow(hwnd);
		UnregisterClass(WINDOW_CLASS_NAME, Instance);
	}
	
	HINSTANCE Instance;
	HWND hwnd;
};
//-----------------------------------------------------------------------------
static stl::vector<MonitorDesc> gMonitors;
static uint32_t gMonitorCount = 0;
static bool gWindowCreated = false;
//-----------------------------------------------------------------------------
inline void getRecommendedResolution(RectDesc *rect)
{
	*rect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}
//-----------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if ( !gWindowCreated )
		return DefWindowProc(hWnd, msg, wParam, lParam);

	switch ( msg )
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
//-----------------------------------------------------------------------------
static BOOL CALLBACK monitorCallback(HMONITOR pMonitor, HDC pDeviceContext, LPRECT pRect, LPARAM pParam)
{
	MONITORINFO info;
	info.cbSize = sizeof(info);
	GetMonitorInfo(pMonitor, &info);
	unsigned index = (unsigned)pParam;

	gMonitors[index].monitorRect = { (int)info.rcMonitor.left, (int)info.rcMonitor.top, (int)info.rcMonitor.right,
									 (int)info.rcMonitor.bottom };
	gMonitors[index].workRect = { (int)info.rcWork.left, (int)info.rcWork.top, (int)info.rcWork.right, (int)info.rcWork.bottom };

	return TRUE;
}
//-----------------------------------------------------------------------------
bool Window::init()
{
	m_pimpl = new Window::WindowPlatform;
	m_pimpl->Instance = GetModuleHandle(nullptr);
	if ( !instance )
	{
		FatalError("Failed to get module handle");
		return false;
	}		

	WNDCLASS wndClass;
	memset(&wndClass, 0, sizeof(wndClass));
	wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.hInstance = m_pimpl->Instance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName = WINDOW_CLASS_NAME;
	if ( !RegisterClass(&wndClass) )
	{
		DWORD errorMessageID = ::GetLastError();
		if ( errorMessageID != ERROR_CLASS_ALREADY_EXISTS )
		{
			LPSTR messageBuffer = NULL;
			size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
			stl::string message(messageBuffer, size);
			FatalError(message.c_str());
			return false;
		}
	}	

	collectMonitorInfo();

	if ( m_config.width == 0 || m_config.height == 0 )
	{
		RectDesc rect = {};
		getRecommendedResolution(&rect);
		m_config.width = GetRectWidth(rect);
		m_config.height = GetRectHeight(rect);
	}

	DWORD style = WS_OVERLAPPEDWINDOW;
	DWORD styleEX = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	m_windowedRect = { 0, 0, (int)m_config.width, (int)m_config.height };
	RECT clientRect = { (LONG)m_windowedRect.left, (LONG)m_windowedRect.top, (LONG)m_windowedRect.right, (LONG)m_windowedRect.bottom };
	AdjustWindowRectEx(&clientRect, style, false, styleEX);

	m_windowedRect = { (int)clientRect.left, (int)clientRect.top, (int)clientRect.right, (int)clientRect.bottom };	

	m_fullscreenRect = { 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };

	RectDesc &rect = m_config.fullscreen ? m_fullscreenRect : m_windowedRect;
	
	TODO("проверить fullscreen - позиция?");
	DWORD screenLeft = (GetSystemMetrics(SM_CXSCREEN) >> 1) - (m_config.width >> 1);
	DWORD screenTop = (GetSystemMetrics(SM_CYSCREEN) >> 1) - (m_config.height >> 1);
	int screenHalfX = (int)GetSystemMetrics(SM_CXSCREEN) >> 1;
	int screenHalfY = (int)GetSystemMetrics(SM_CYSCREEN) >> 1;	
	m_pimpl->hwnd = CreateWindowEx(styleEX, WINDOW_CLASS_NAME, TEXT("Game"), WS_CLIPSIBLINGS | WS_CLIPCHILDREN | style, screenLeft, screenTop, (rect.right - rect.left), (rect.bottom - rect.top), NULL, NULL, m_pimpl->Instance, NULL);
	if ( !m_pimpl->hwnd )
	{
		FatalError("Failed to create window app ");
		return false;
	}

	GetClientRect(m_pimpl->hwnd, &clientRect);
	m_windowedRect = { (int)clientRect.left, (int)clientRect.top, (int)clientRect.right, (int)clientRect.bottom };

	if ( m_config.fullscreen )
	{
		// Make the window borderless so that the client area can fill the screen.
		SetWindowLong(m_pimpl->hwnd, GWL_STYLE, WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);

		// Get the settings of the primary display. We want the app to go into
		// fullscreen mode on the display that supports Independent Flip.
		DEVMODE devMode = {};
		devMode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);

		SetWindowPos(m_pimpl->hwnd, HWND_TOPMOST, devMode.dmPosition.x, devMode.dmPosition.y, devMode.dmPosition.x + devMode.dmPelsWidth, devMode.dmPosition.y + devMode.dmPelsHeight, SWP_FRAMECHANGED | SWP_NOACTIVATE);

		ShowWindow(m_pimpl->hwnd, SW_MAXIMIZE);
	}
	else
	{
		if ( m_config.maximized )
			ShowWindow(m_pimpl->hwnd, SW_MAXIMIZE);
		else
			ShowWindow(m_pimpl->hwnd, SW_NORMAL);
	}
	SetForegroundWindow(m_pimpl->hwnd);
	SetFocus(m_pimpl->hwnd);
	UpdateWindow(m_pimpl->hwnd);

	m_config.width = m_config.fullscreen ? GetRectWidth(m_fullscreenRect) : GetRectWidth(m_windowedRect);
	m_config.height = m_config.fullscreen ? GetRectHeight(m_fullscreenRect) : GetRectHeight(m_windowedRect);

	gWindowCreated = true;

	return true;
}
//-----------------------------------------------------------------------------
void Window::close()
{
	gMonitors.clear();
	ShowCursor(true);	
	SafeDelete(m_pimpl);
	gWindowCreated = false;
}
//-----------------------------------------------------------------------------
void Window::collectMonitorInfo()
{
	DISPLAY_DEVICEW adapter;
	adapter.cb = sizeof(adapter);

	int found = 0;
	int size = 0;
	uint32_t monitorCount = 0;

	for ( int adapterIndex = 0;; ++adapterIndex )
	{
		if ( !EnumDisplayDevices(NULL, adapterIndex, &adapter, 0) )
			break;

		if ( !(adapter.StateFlags & DISPLAY_DEVICE_ACTIVE) )
			continue;


		for ( int displayIndex = 0;; displayIndex++ )
		{
			DISPLAY_DEVICEW display;
			display.cb = sizeof(display);

			if ( !EnumDisplayDevices(adapter.DeviceName, displayIndex, &display, 0) )
				break;

			++monitorCount;
		}
	}

	gMonitorCount = monitorCount;
	gMonitors.resize(monitorCount);

	for ( int adapterIndex = 0;; ++adapterIndex )
	{
		if ( !EnumDisplayDevicesW(NULL, adapterIndex, &adapter, 0) )
			break;

		if ( !(adapter.StateFlags & DISPLAY_DEVICE_ACTIVE) )
			continue;

		for ( int displayIndex = 0;; displayIndex++ )
		{
			DISPLAY_DEVICEW display;
			HDC dc;

			display.cb = sizeof(display);

			if ( !EnumDisplayDevicesW(adapter.DeviceName, displayIndex, &display, 0) )
				break;

			dc = CreateDCW(L"DISPLAY", adapter.DeviceName, NULL, NULL);

			MonitorDesc desc;
			desc.modesPruned = (adapter.StateFlags & DISPLAY_DEVICE_MODESPRUNED) != 0;

			wcsncpy_s(desc.adapterName, adapter.DeviceName, elementsOf(adapter.DeviceName));
			wcsncpy_s(desc.publicAdapterName, adapter.DeviceName, elementsOf(adapter.DeviceName));
			wcsncpy_s(desc.displayName, display.DeviceName, elementsOf(display.DeviceName));
			wcsncpy_s(desc.publicDisplayName, display.DeviceName, elementsOf(display.DeviceName));

			gMonitors[found] = (desc);
			EnumDisplayMonitors(NULL, NULL, monitorCallback, found);

			DeleteDC(dc);

			if ( (adapter.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) && displayIndex == 0 )
			{
				MonitorDesc desc = gMonitors[0];
				gMonitors[0] = gMonitors[found];
				gMonitors[found] = desc;
			}

			found++;
		}
	}

	for ( uint32_t monitor = 0; monitor < monitorCount; ++monitor )
	{
		MonitorDesc* pMonitor = &gMonitors[monitor];
		DEVMODEW devMode = {};
		devMode.dmSize = sizeof(DEVMODEW);
		devMode.dmFields = DM_PELSHEIGHT | DM_PELSWIDTH;

		EnumDisplaySettingsW(pMonitor->adapterName, ENUM_CURRENT_SETTINGS, &devMode);
		pMonitor->defaultResolution.Height = devMode.dmPelsHeight;
		pMonitor->defaultResolution.Width = devMode.dmPelsWidth;

		stl::vector<Resolution> displays;
		DWORD current = 0;
		while ( EnumDisplaySettingsW(pMonitor->adapterName, current++, &devMode) )
		{
			bool duplicate = false;
			for ( uint32_t i = 0; i < (uint32_t)displays.size(); ++i )
			{
				if ( displays[i].Width == (uint32_t)devMode.dmPelsWidth && displays[i].Height == (uint32_t)devMode.dmPelsHeight )
				{
					duplicate = true;
					break;
				}
			}

			if ( duplicate )
				continue;

			Resolution videoMode = {};
			videoMode.Height = devMode.dmPelsHeight;
			videoMode.Width = devMode.dmPelsWidth;
			displays.emplace_back(videoMode);
		}
		qsort(displays.data(), displays.size(), sizeof(Resolution), [](const void* lhs, const void* rhs) {
			Resolution* pLhs = (Resolution*)lhs;
			Resolution* pRhs = (Resolution*)rhs;
			if ( pLhs->Height == pRhs->Height )
				return (int)(pLhs->Width - pRhs->Width);

			return (int)(pLhs->Height - pRhs->Height);
			});

		pMonitor->resolutionCount = (uint32_t)displays.size();
		pMonitor->resolutions.resize(pMonitor->resolutionCount);
		memcpy(pMonitor->resolutions.data(), displays.data(), pMonitor->resolutionCount * sizeof(Resolution));
	}
}
//-----------------------------------------------------------------------------
void Window::Update()
{
	MSG msg;
	if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if ( msg.message == WM_QUIT || msg.message == WM_CLOSE )
			Application::Exit();
	}
}
//-----------------------------------------------------------------------------
void Window::Swap()
{
}
//-----------------------------------------------------------------------------
#endif