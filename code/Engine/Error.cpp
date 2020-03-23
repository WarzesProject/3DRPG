#include "stdafx.h"
#include "Error.h"
#include "Log.h"
//-----------------------------------------------------------------------------
#if !SE_ENABLE_EXCEPTION
bool bFatalError = false;
#endif
//-----------------------------------------------------------------------------
#if !SE_ENABLE_EXCEPTION
bool IsFatalError()
{
	return bFatalError;
}
#endif
//-----------------------------------------------------------------------------
void FatalError(std::string_view str)
{
#if SE_ENABLE_EXCEPTION
	throw Exception(str);
#else
	SE_LOG_ERROR(str);
	bFatalError = true;
#endif
}
//-----------------------------------------------------------------------------