#include "stdafx.h"
#include "Error.h"
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
	//Logs::Error(str);
	bFatalError = true;
#endif
}
//-----------------------------------------------------------------------------