#pragma once

#include "foundation.h"
#include "Singleton.hpp"
#include "Thread.h"

typedef time_t Time;
typedef __time64_t Tick;

#define DATETIME_FORMAT "D%Y%m%dT%H%M%S"
#define DATE_FORMAT "%Y-%m-%d"
#define TIME_FORMAT "%H:%M:%S"

class DateTime : public Singleton<DateTime>, public Thread
{
public:
	DateTime();
	~DateTime();

	// dateStr을 가지고 dateformat에 맞게 Time 리턴해준다.
	Time StringToTime(const std::string& IN datestr, const char* dateformat);
	std::string TimeToString(const Time& time, const char* datetime_format);

	std::string nowTime(const char* datetime_format);
private:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;

	enum
	{
		eMAX_STRING = 10000,
	};
private:
	std::atomic<Tick> _systemTick;
	std::atomic<bool> _threadstoprequest;
	Tick			  _startTick;
};

