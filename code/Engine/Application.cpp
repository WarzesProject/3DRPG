#include "stdafx.h"
#include "Application.h"
#include "Log.h"
//-----------------------------------------------------------------------------
struct AppPimpl
{
	Configuration config;
};
//-----------------------------------------------------------------------------
Application::Application()
	: m_impl(new AppPimpl)
{
}
//-----------------------------------------------------------------------------
Application::~Application()
{
	close();
	SafeDelete(m_impl);
}
//-----------------------------------------------------------------------------
bool Application::init(const Configuration &config)
{
	m_impl->config = config;

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
	auto &config = m_impl->config;

#if SE_ENABLE_EXCEPTION
#	define SE_INIT_SUBSYSTEM(_ss) ((isError) = (isError) || !(_ss))
#else
#	define SE_INIT_SUBSYSTEM(_ss) ((isError) = ((isError) || (IsErrorCritical()) || !(_ss)))
#endif

	//SE_INIT_SUBSYSTEM(Logger::Create());

#undef SE_INIT_SUBSYSTEM

	return !isError;
}
//-----------------------------------------------------------------------------
void Application::deltaTime()
{
}
//-----------------------------------------------------------------------------
bool Application::beginFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	return true;
}
//-----------------------------------------------------------------------------
bool Application::update()
{
	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
}
//-----------------------------------------------------------------------------