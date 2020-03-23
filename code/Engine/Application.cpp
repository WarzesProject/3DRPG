#include "stdafx.h"
#include "Application.h"
#include "Window.h"
#include <gl/GL.h>
//-----------------------------------------------------------------------------
struct Application::AppPimpl
{
	AppPimpl(Window &wnd)
		: window(wnd)
	{
	}

	Window &window;
};
//-----------------------------------------------------------------------------
Application::Application()
{
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	close();
	SafeDelete(m_pimpl);
}
//-----------------------------------------------------------------------------
bool Application::init(const Configuration &config)
{
	m_config = config;

#if SE_DEBUG && SE_PLATFORM_WINDOWS
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	if( !initSubsystem() )
		return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::initSubsystem()
{
	bool isError = false;

#if SE_ENABLE_EXCEPTION
#	define SE_INIT_SUBSYSTEM(_ss) ((isError) = (isError) || !(_ss))
#else
#	define SE_INIT_SUBSYSTEM(_ss) ((isError) = ((isError) || (IsFatalError()) || !(_ss)))
#endif

	SE_INIT_SUBSYSTEM(Window::Create(m_config.window));

#undef SE_INIT_SUBSYSTEM


	m_pimpl = new Application::AppPimpl(GetSubsystem<Window>());

	return !isError;
}
//-----------------------------------------------------------------------------
void Application::deltaTime()
{
}
//-----------------------------------------------------------------------------
bool Application::beginFrame()
{
	glViewport(0, 0, 800, 600);
	glClearColor(0.2f, 0.4f, 0.9f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	m_pimpl->window.Swap();
	return true;
}
//-----------------------------------------------------------------------------
bool Application::update()
{
	m_pimpl->window.Update();

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
	Window::Destroy();
}
//-----------------------------------------------------------------------------