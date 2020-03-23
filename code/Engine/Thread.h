#pragma once

class Thread
{
public:
	Thread() noexcept = default;
	Thread(const Thread&) = delete;
	Thread(Thread &&other) noexcept : m_thread(stl::move(other.m_thread)) {}
	Thread(const stl::thread&) = delete;
	Thread(stl::thread &&other) noexcept : m_thread(stl::move(other)) {}

	Thread& operator=(const Thread&) = delete;
	Thread& operator=(Thread &&other) noexcept
	{
		m_thread = stl::move(other.m_thread);
		return *this;
	}

	Thread& operator=(const stl::thread&) = delete;
	Thread& operator=(stl::thread &&other) noexcept
	{
		m_thread = std::move(other);
		return *this;
	}

	template <typename Callable, typename... Args>
	explicit Thread(Callable &&f, Args &&... args) :
		m_thread(stl::forward<Callable>(f), stl::forward<Args>(args)...)
	{
	}

	~Thread()
	{
		if ( m_thread.joinable() ) m_thread.join();
	}

	auto IsJoinable() const
	{
		return m_thread.joinable();
	}
	auto GetId() const noexcept
	{
		return m_thread.get_id();
	}
	auto GetNativeHandle()
	{
		return m_thread.native_handle();
	}

	void Join()
	{
		m_thread.join();
	}

private:
	stl::thread m_thread;
};