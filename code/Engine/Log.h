#pragma once

#if !SE_PLATFORM_EMSCRIPTEN
#	include "Engine/Thread.h"
#endif

class Logger;

class Log
{
public:
	enum class Level
	{
		Off,
		Error,
		Warning,
		Info,
		All
	};

	explicit Log(const Logger &initLogger, Level initLevel = Level::Info)
		: m_logger(initLogger)
		, m_level(initLevel)
	{
	}

	Log(const Log &other)
		: m_logger(other.m_logger)
		, m_level(other.m_level)
		, m_str(other.m_str)
	{
	}

	Log(Log &&other) noexcept
		: m_logger(other.m_logger)
		, m_level(other.m_level)
		, m_str(std::move(other.m_str))
	{
		other.m_level = Level::Info;
	}

	Log& operator=(const Log &other)
	{
		if ( &other == this ) return *this;

		m_level = other.m_level;
		m_str = other.m_str;

		return *this;
	}

	Log& operator=(Log &&other) noexcept
	{
		if ( &other == this ) return *this;

		m_level = other.m_level;
		other.m_level = Level::Info;
		m_str = std::move(other.m_str);

		return *this;
	}

	~Log();

	template <typename T, typename stl::enable_if<stl::is_same<T, bool>::value>::type * = nullptr>
	Log& operator<<(const T val)
	{
		m_str += val ? "true" : "false";
		return *this;
	}

	template <typename T, typename stl::enable_if<stl::is_same<T, uint8_t>::value>::type * = nullptr>
	Log& operator<<(const T val)
	{
		constexpr char digits[] = "0123456789abcdef";

		for ( uint32_t p = 0; p < 2; ++p )
			m_str.push_back(digits[(val >> (4 - p * 4)) & 0x0F]);

		return *this;
	}

	template <typename T, typename stl::enable_if<stl::is_arithmetic<T>::value &&
		!stl::is_same<T, bool>::value &&
		!stl::is_same<T, uint8_t>::value>::type * = nullptr>
		Log& operator<<(const T val)
	{
		m_str += stl::to_string(val);
		return *this;
	}

	template <typename T, typename stl::enable_if<stl::is_same<T, stl::string>::value>::type * = nullptr>
	Log& operator<<(const T &val)
	{
		m_str += val;
		return *this;
	}

	template <typename T, typename stl::enable_if<stl::is_same<T, stl::string_view>::value>::type * = nullptr>
	Log& operator<<(const T &val)
	{
		m_str += val.data();
		return *this;
	}


	template <typename T, typename stl::enable_if<stl::is_same<T, char>::value>::type * = nullptr>
	Log& operator<<(const T *val)
	{
		m_str += val;
		return *this;
	}

	template <typename T, typename stl::enable_if<!stl::is_same<T, char>::value>::type * = nullptr>
	Log& operator<<(const T *val)
	{
		constexpr char digits[] = "0123456789abcdef";

		stl::uintptr_t ptrValue;
		memcpy(&ptrValue, &val, sizeof(ptrValue));

		for ( size_t i = 0; i < sizeof(val) * 2; ++i )
			m_str.push_back(digits[(ptrValue >> (sizeof(ptrValue) * 2 - i - 1) * 4) & 0x0F]);

		return *this;
	}

private:
	const Logger &m_logger;
	Level m_level = Level::Info;
	std::string m_str;
};

class Logger final
{
public:
	explicit Logger(Log::Level initThreshold = Log::Level::All)
		: m_threshold(initThreshold)
	{
#if !SE_PLATFORM_EMSCRIPTEN
		m_logThread = Thread(&Logger::logLoop, this);
#endif
	}

	Logger(const Logger&) = delete;
	Logger& operator=(const Logger&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger&&) = delete;

	~Logger()
	{
#if !SE_PLATFORM_EMSCRIPTEN
		std::unique_lock<std::mutex> lock(m_queueMutex);
		m_commandQueue.push(Command(Command::Type::Quit));
		lock.unlock();
		m_queueCondition.notify_all();
#endif
	}

	Log GetLog(const Log::Level level = Log::Level::Info) const
	{
		return Log(*this, level);
	}

	void Print(const stl::string &str, const Log::Level level = Log::Level::Info) const
	{
		if ( level <= m_threshold )
		{
#if SE_PLATFORM_EMSCRIPTEN
			logString(str, level);
#else
			stl::unique_lock<stl::mutex> lock(m_queueMutex);
			m_commandQueue.push(Command(Command::Type::LogString, level, str));
			lock.unlock();
			m_queueCondition.notify_all();
#endif
		}
	}

private:
	static void logString(std::string_view str, const Log::Level level = Log::Level::Info);

#if SE_DEBUG
	stl::atomic<Log::Level> m_threshold{ Log::Level::All };
#else
	stl::atomic<Log::Level> threshold{ Log::Level::Info };
#endif

#if !SE_PLATFORM_EMSCRIPTEN
	class Command
	{
	public:
		enum class Type
		{
			LogString,
			Quit
		};

		explicit Command(const Type initType) : type(initType) {}
		Command(const Type initType, const Log::Level initLevel, const stl::string &initString) 
			: type(initType)
			, level(initLevel)
			, str(initString)
		{
		}

		Type type;
		Log::Level level;
		std::string str;
	};

	void logLoop()
	{
		for ( ;;)
		{
			stl::unique_lock<stl::mutex> lock(m_queueMutex);
			while ( m_commandQueue.empty() ) m_queueCondition.wait(lock);
			auto command = stl::move(m_commandQueue.front());
			m_commandQueue.pop();
			lock.unlock();

			if ( command.type == Command::Type::LogString )
				logString(command.str, command.level);
			else if ( command.type == Command::Type::Quit )
				break;
		}
	}

	mutable stl::condition_variable m_queueCondition;
	mutable stl::mutex m_queueMutex;
	mutable stl::queue<Command> m_commandQueue;
	Thread m_logThread;
#endif
};

extern Logger DefaultLog;

#define SE_LOG(level, str) {DefaultLog.GetLog((level)) << (str);}
#define SE_LOG_ERROR(str) {DefaultLog.GetLog(Log::Level::Error) << (str);}
#define SE_LOG_WARNING(str) {DefaultLog.GetLog(Log::Level::Warning) << (str);}
#define SE_LOG_INFO(str) {DefaultLog.GetLog(Log::Level::Info) << (str);}