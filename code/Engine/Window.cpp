#include "stdafx.h"
#include "Window.h"
//-----------------------------------------------------------------------------
Window::Window(WindowConfiguration &config)
	: m_config(config)
{
	if ( !init() )
		return;

	setValid(true);
}
//-----------------------------------------------------------------------------
Window::~Window()
{
	close();
}
//-----------------------------------------------------------------------------