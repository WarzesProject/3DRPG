#pragma once

#if SE_ENABLE_EXCEPTION
struct Exception : public std::exception
{
	Exception(std::string_view str) : std::exception(str.data()){}
};
#else
bool IsFatalError();
#endif

void FatalError(stl::string_view str);