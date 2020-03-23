#include "stdafx.h"
#include "Log.h"
//-----------------------------------------------------------------------------
Logger DefaultLog;
//-----------------------------------------------------------------------------
Log::~Log()
{
	if ( !m_str.empty() )
		m_logger.Print(m_str, m_level);
}
//-----------------------------------------------------------------------------
void Logger::logString(stl::string_view str, Log::Level level)
{
#if SE_PLATFORM_ANDROID
	int priority = 0;
	switch ( level )
	{
	case Log::Level::Error: priority = ANDROID_LOG_ERROR; break;
	case Log::Level::Warning: priority = ANDROID_LOG_WARN; break;
	case Log::Level::Info: priority = ANDROID_LOG_INFO; break;
	case Log::Level::All: priority = ANDROID_LOG_DEBUG; break;
	default: return;
	}
	__android_log_print(priority, "Sapphire", "%s", str.data());
#elif SE_PLATFORM_LINUX
	int fd = 0;
	switch ( level )
	{
	case Log::Level::Error:
	case Log::Level::Warning:
		fd = STDERR_FILENO;
		break;
	case Log::Level::Info:
	case Log::Level::All:
		fd = STDOUT_FILENO;
		break;
	default: return;
	}

	std::vector<char> output(str.begin(), str.end());
	output.push_back('\n');

	size_t offset = 0;
	while ( offset < output.size() )
	{
		const ssize_t written = write(fd, output.data() + offset, output.size() - offset);
		if ( written == -1 )
			return;

		offset += static_cast<size_t>(written);
	}
#elif SE_PLATFORM_WINDOWS
	const int bufferSize = MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, nullptr, 0);
	if ( bufferSize == 0 )
		return;

	std::vector<WCHAR> buffer(bufferSize + 1); // +1 for the newline
	if ( MultiByteToWideChar(CP_UTF8, 0, str.data(), -1, buffer.data(), static_cast<int>(buffer.size())) == 0 )
		return;

	if ( FAILED(StringCchCatW(buffer.data(), buffer.size(), L"\n")) )
		return;

	OutputDebugStringW(buffer.data());
#   if SE_DEBUG
	HANDLE handle;
	switch ( level )
	{
	case Log::Level::Error:
	case Log::Level::Warning:
		handle = GetStdHandle(STD_ERROR_HANDLE);
		break;
	case Log::Level::Info:
	case Log::Level::All:
		handle = GetStdHandle(STD_OUTPUT_HANDLE);
		break;
	default: return;
	}

	DWORD bytesWritten;
	WriteConsoleW(handle, buffer.data(), static_cast<DWORD>(wcslen(buffer.data())), &bytesWritten, nullptr);
#   endif
#elif SE_PLATFORM_EMSCRIPTEN
	int flags = EM_LOG_CONSOLE;
	switch ( level )
	{
	case Log::Level::Error:
		flags |= EM_LOG_ERROR;
		break;
	case Log::Level::Warning:
		flags |= EM_LOG_WARN;
		break;
	case Log::Level::Info:
	case Log::Level::All:
		break;
	default: return;
	}
	emscripten_log(flags, "%s", str.data());
#endif
}
//-----------------------------------------------------------------------------