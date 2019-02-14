
#include "time.h"

namespace Timing
{
	// wMilliseconds는 사용되지 않음.

	bool SystemTimeToTimet(const SYSTEMTIME& IN val, time_t& OUT timet)
	{
		struct tm convertedtm;

		convertedtm.tm_year = val.wYear - 1900;
		convertedtm.tm_mon = val.wMonth - 1;
		convertedtm.tm_wday = val.wDayOfWeek;
		convertedtm.tm_mday = val.wDay;
		convertedtm.tm_hour = val.wHour;
		convertedtm.tm_min = val.wMinute;
		convertedtm.tm_sec = val.wSecond;

		timet = mktime(&convertedtm);
		return (timet != -1);

	}

	bool TimetToSystemTime(const time_t& IN val, SYSTEMTIME& OUT systemtime)
	{
		struct tm convertedtm;
		errno_t err = localtime_s(&convertedtm, &val);
		if (err != 0)
			return false;

		systemtime.wYear = convertedtm.tm_year + 1900;
		systemtime.wMonth = convertedtm.tm_mon + 1;
		systemtime.wDayOfWeek = convertedtm.tm_wday;
		systemtime.wDay = convertedtm.tm_mday;
		systemtime.wHour = convertedtm.tm_hour;
		systemtime.wMinute = convertedtm.tm_min;
		systemtime.wSecond = convertedtm.tm_sec;

		systemtime.wMilliseconds = 0;


		return true;

	}

	bool TimetToVariantTime(const time_t& val, double& OUT varianttime)
	{
		varianttime = 0;

		SYSTEMTIME systemtime;
		if (false == TimetToSystemTime(val, systemtime))
			return false;

		if (false == SystemTimeToVariantTime(&systemtime, &varianttime))
			return false;

		return true;
	}

	// 31일이 없어도 동작함.
	bool StringToSystemTime(const std::string& IN dateString, SYSTEMTIME& OUT val)
	{
		ZeroMemory(&val, sizeof(SYSTEMTIME));
		int arg = sscanf_s(dateString.c_str(), "%04hu-%02hu-%02hu %02hu:%02hu:%02hu:.%03hu",
			&val.wYear, &val.wMonth, &val.wDay, &val.wHour, &val.wMinute, &val.wSecond, &val.wMilliseconds);

		if (arg == 7)
		{
			time_t check;
			return SystemTimeToTimet(val, check);
		}

		return false;
	}

	bool StringToTimeT(const std::string& IN dateString, time_t& OUT val)
	{
		val = 0;
		struct tm timest;
		ZeroMemory(&timest, sizeof(struct tm));

		int arg = sscanf_s(dateString.c_str(), "%04d-%02d-%02d %02d:%02d:%02d",
			&timest.tm_year, &timest.tm_mon, &timest.tm_mday,
			&timest.tm_hour, &timest.tm_min, &timest.tm_sec);

		if (arg == 6)
		{
			timest.tm_year -= 1990;
			timest.tm_mon -= 1;
			timest.tm_isdst -= 1;

			val = mktime(&timest);
			return (val != -1);
		}

		return false;
	}

	// _variant_t.date는 double,
	// db상의 datetime을 double로 변환한 값과 동일함.
	// datetime형은 milliseconds 정확도 이지만 double로 변환시, 초단위로 짤림

	bool VariantTimeToTimet(const double& IN val, time_t& OUT timet)
	{
		SYSTEMTIME systemTime;
		if (false == VariantTimeToSystemTime(val, &systemTime))
			return false;

		struct tm tmst;
		tmst.tm_year = systemTime.wYear - 1900;
		tmst.tm_mon = systemTime.wMonth - 1;
		tmst.tm_wday = systemTime.wDayOfWeek;
		tmst.tm_mday = systemTime.wDay;
		tmst.tm_hour = systemTime.wHour;
		tmst.tm_min = systemTime.wMinute;
		tmst.tm_sec = systemTime.wSecond;

		timet = mktime(&tmst);
		return (timet != -1);
	}

	std::string GetLocalTimeString(bool useMilliSec /*= false*/, char colon /*= ':'*/)
	{
		char buffer[30];
		SYSTEMTIME t;

		::GetLocalTime(&t);

		if (true == useMilliSec)
		{
			sprintf_s(buffer, "%04d-%02d-%02d %02d%c%02d%c%02d.%03d",
				t.wYear, t.wMonth, t.wDay, t.wHour, colon, t.wMinute, colon,
				t.wSecond, t.wMilliseconds);
		}
		else
		{
			sprintf_s(buffer, "%04d-%02d-%02d %02d%c%02d%c%02d",
				t.wYear, t.wMonth, t.wDay, t.wHour, colon, t.wMinute, colon,
				t.wSecond);
		}

		return buffer;
	}

	std::string GetLocalTimeStringBracket(bool useMilliSec /*= false*/, char colon /*= ':'*/)
	{
		char buffer[30];
		SYSTEMTIME t;

		::GetLocalTime(&t);

		if (true == useMilliSec)
		{
			sprintf_s(buffer, "[%04d-%02d-%02d %02d%c%02d%c%02d.%03d]",
				t.wYear, t.wMonth, t.wDay, t.wHour, colon, t.wMinute, colon,
				t.wSecond, t.wMilliseconds);
		}
		else
		{
			sprintf_s(buffer, "[%04d-%02d-%02d %02d%c%02d%c%02d]",
				t.wYear, t.wMonth, t.wDay, t.wHour, colon, t.wMinute, colon,
				t.wSecond);
		}

		return buffer;
	}

	// faction은 무시된다.
	bool TimestampStructToTimet(SQL_TIMESTAMP_STRUCT& IN val, time_t& OUT timet)
	{
		struct tm converttm;
		converttm.tm_year = val.year - 1900;
		converttm.tm_mon = val.month - 1;
		converttm.tm_mday = val.day;
		converttm.tm_hour = val.hour;
		converttm.tm_min = val.minute;
		converttm.tm_sec = val.second;

		timet = mktime(&converttm);
		return (timet != -1);
	}


}

