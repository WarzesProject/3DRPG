#pragma once

//=============================================================================
// Base header
#include "Engine/Version.h"
#include "Engine/EngineConfig.h"
#include "Engine/DetectCompiler.h"
#include "Engine/DetectOS.h"
#include "Engine/PostConfigEngine.h"
#include "Engine/BaseMacros.h"
#include "Engine/InlineFunc.h"

//=============================================================================
// STL
#pragma warning( push, 1 )

#if SE_ENABLE_EXCEPTION
#	include <exception>
#endif

#include <ctime>
#if SE_ENABLE_EASTL
#else
#	include <algorithm>
#	include <string>
#	include <string_view>
#	include <queue>
#	include <atomic>
#	include <thread>
#	include <condition_variable>

#endif
#pragma warning( pop )

namespace stl
{
#if SE_ENABLE_EASTL
	using namespace eastl;
#else
	using namespace std;
#endif
}

//=============================================================================
// Platform header
#if SE_PLATFORM_WINDOWS
#	include "Engine/Win32PlatformDefinitions.h"
#	pragma warning( push, 1 )
#	include <Windows.h>
#	include <strsafe.h>
#	pragma warning( pop )
#endif
#include "Engine/PlatformMacros.h"

//=============================================================================
// 3rdparty header

//=============================================================================
// Other header