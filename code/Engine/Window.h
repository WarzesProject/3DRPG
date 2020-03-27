#pragma once

#include "Engine/WindowConfiguration.h"
#include "Engine/Subsystem.h"

typedef struct RectDesc
{
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
} RectDesc;
inline int GetRectWidth(const RectDesc &rect) { return rect.right - rect.left; }
inline int GetRectHeight(const RectDesc &rect) { return rect.bottom - rect.top; }

typedef struct Resolution
{
	uint32_t Width;
	uint32_t Height;
} Resolution;

typedef struct MonitorDesc
{
	RectDesc monitorRect;
	RectDesc workRect;
	// This size matches the static size of DISPLAY_DEVICE.DeviceName
#if SE_PLATFORM_WINDOWS
	WCHAR adapterName[32];
	WCHAR displayName[32];
	WCHAR publicAdapterName[64];
	WCHAR publicDisplayName[64];
#else
	char  adapterName[32];
	char  displayName[32];
	char  publicAdapterName[64];
	char  publicDisplayName[64];
#endif
	bool modesPruned;
	bool modeChanged;

	Resolution  defaultResolution;
	std::vector<Resolution> resolutions;
	uint32_t    resolutionCount;
} MonitorDesc;

class Window : public Subsystem<Window>
{
public:
	Window(WindowConfiguration &config);
	~Window();

	void Update();
	void Swap();
private:
	bool init();
	void close();
	void collectMonitorInfo();
	WindowConfiguration &m_config;
	
	struct WindowPlatform;
	WindowPlatform *m_pimpl;

	RectDesc m_windowedRect;
	RectDesc m_fullscreenRect;
};