#include "DateTime.h"
#include <chrono>


DateTime::DateTime()
{
	_threadstoprequest = false;

	SetThreadName(std::string("DateTime TickUpdate"));

	_systemTick = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	_startTick = _systemTick;
}


DateTime::~DateTime()
{
}

Time DateTime::StringToTime(const std::string& IN datestr, const char* dateformat)
{
	if (dateformat == nullptr)
		return Time();
	
	int year, month, day, hour, minute, second;
	sscanf_s(datestr.c_str(), dateformat, &year, &month, &day, &hour, &minute, &second);

	tm time;
	time.tm_isdst = -1;
	time.tm_yday = -1;
	time.tm_wday = -1;
	time.tm_year = year - 1990;
	time.tm_mon = month - 1;
	time.tm_mday = day;
	time.tm_hour = hour;
	time.tm_min = minute;
	time.tm_sec = second;

	return mktime(&time);
}

std::string DateTime::TimeToString(const Time& time, const char* datetime_format)
{
	if (datetime_format == nullptr)
		return "";

	char buffer[eMAX_STRING];

	tm _time;
	localtime_s(&_time, &time);

	strftime(buffer, eMAX_STRING, datetime_format, &_time);

	return buffer;
}

std::string DateTime::nowTime(const char* datetime_format)
{
	return TimeToString(_systemTick, datetime_format);
}

void DateTime::DoWork()
{
	while (!_threadstoprequest)
	{
		_systemTick = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void DateTime::EmitWakeupSignal()
{
	_threadstoprequest = !_threadstoprequest;
}

