#pragma once

#include "foundation.h"

namespace Timing
{
	using Time = double;
	using Tick = __int64;

	inline Time TicksToSeconds(Tick ticks)
	{
		return ticks * 0.001;
	}

	inline Time SecondsToTicks(Time t)
	{
		return Time((t * 1000.0) + 0.5); // ¹Ý¿Ã¸²
	}

	bool SystemTimeToTimet(const SYSTEMTIME& IN val, time_t& OUT timet);
	
	bool TimetToSystemTime(const time_t& IN val, SYSTEMTIME& OUT systemtime);
	
	bool TimetToVariantTime(const time_t& val, double& OUT varianttime);

	bool StringToSystemTime(const std::string& IN dateString, SYSTEMTIME& OUT val);

	bool StringToTimeT(const std::string& IN dateString, time_t& OUT val);

	bool VariantTimeToTimet(const double& IN val, time_t& OUT timet);

	std::string GetLocalTimeString(bool useMilliSec = false, char colon = ':');

	std::string GetLocalTimeStringBracket(bool useMilliSec = false, char colon = ':');

	bool TimestampStructToTimet(SQL_TIMESTAMP_STRUCT& IN val, time_t& OUT timet);

}