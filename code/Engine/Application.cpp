#include "stdafx.h"
#include "Application.h"
#include "Window.h"
#include "RenderSystem.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
struct Application::AppPimpl
{
	AppPimpl(Window &wnd, RenderSystem &rs)
		: window(wnd)
		, render(rs)
	{
	}

	Window &window;
	RenderSystem &render;
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

	m_pimpl = new Application::AppPimpl(
		GetSubsystem<Window>(),
		GetSubsystem<RenderSystem>());

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
	SE_INIT_SUBSYSTEM(RenderSystem::Create(m_config.render));

#undef SE_INIT_SUBSYSTEM

	return !isError;
}
//-----------------------------------------------------------------------------
void Application::deltaTime()
{
	m_deltaTime = m_deltaTimer.GetMSec(true) / 1000.0f;
	// if framerate appears to drop below about 6, assume we're at a breakpoint and simulate 20fps.
	if ( m_deltaTime > 0.15f )
		m_deltaTime = 0.05f;
}
//-----------------------------------------------------------------------------
bool Application::beginFrame()
{
	m_pimpl->render.BeginFrame();
	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	m_pimpl->render.EndFrame();
	m_pimpl->window.Swap();
	return true;
}
//-----------------------------------------------------------------------------
bool Application::update(float dt)
{
	m_pimpl->window.Update();

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
	RenderSystem::Destroy();
	Window::Destroy();
}
//-----------------------------------------------------------------------------