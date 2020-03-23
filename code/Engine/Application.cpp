#include "stdafx.h"
#include "Application.h"


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

#define SE_INIT_SUBSYSTEM(_ss) ((isError) = ((isError) || (IsErrorCriticalExit()) || !(_ss)))

	//SE_INIT_SUBSYSTEM(Platform::Create());

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
	if( IsErrorCriticalExit() ) return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::endFrame()
{
	if( IsErrorCriticalExit() ) return false;

	return true;
}
//-----------------------------------------------------------------------------
bool Application::update()
{
	if( IsErrorCriticalExit() ) return false;

	return true;
}
//-----------------------------------------------------------------------------
void Application::close()
{
}
//-----------------------------------------------------------------------------