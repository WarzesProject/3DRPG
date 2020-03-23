#pragma once

#include "Engine/WindowConfiguration.h"
#include "Engine/Subsystem.h"

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
	WindowConfiguration &m_config;
	
	struct WindowPlatform;
	WindowPlatform *m_pimpl;
};