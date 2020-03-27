#pragma once

class Timer
{
public:
	Timer();
	uint32_t GetMSec(bool reset);
	void Reset();

private:
	uint32_t m_startTime;
};