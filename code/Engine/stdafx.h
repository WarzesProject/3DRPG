#pragma once

#include "Engine/Version.h"
#include "Engine/EngineConfig.h"
#include "Engine/DetectCompiler.h"
#include "Engine/DetectOS.h"
#include "Engine/PostConfigEngine.h"
#include "Engine/BaseMacros.h"
#include "Engine/InlineFunc.h"

#pragma warning( push, 1 )
#include <ctime>
#if SE_ENABLE_EASTL
#else
#	include <algorithm>
#	include <string>
#	include <string_view>
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

#if SE_PLATFORM_WINDOWS
#	include "Engine/Win32PlatformDefinitions.h"
#	pragma warning( push, 1 )
#	include <Windows.h>
#	pragma warning( pop )
#endif
#include "Engine/PlatformMacros.h"