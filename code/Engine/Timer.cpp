#include "stdafx.h"
#include "Timer.h"
//-----------------------------------------------------------------------------
#if SE_PLATFORM_WINDOWS
uint32_t getSystemTime()
{
	return (uint32_t)timeGetTime();
}
#endif
//-----------------------------------------------------------------------------
Timer::Timer()
{
	Reset();
}
//-----------------------------------------------------------------------------
unsigned Timer::GetMSec(bool reset)
{
	unsigned currentTime = getSystemTime();
	unsigned elapsedTime = currentTime - m_startTime;
	if ( reset )
		m_startTime = currentTime;

	return elapsedTime;
}
//-----------------------------------------------------------------------------
void Timer::Reset()
{
	m_startTime = getSystemTime();
}
//-----------------------------------------------------------------------------
