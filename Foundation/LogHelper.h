#pragma once

#include "Logger.h"
#include "StringHelper.h"

namespace Util
{
	class LogHelper
	{
	public:
		template <typename ...Args>
		static void Log(LogLevel level, const std::string& format, Args... args)
		{
			if (sizeof...(args) <= 0)
				Log(level, format);

			const std::string result = StringHelper::Format(format, std::forward<Args>(args)...);

			Logger::Get().EnqueueLog(result, level);
		}

		static void Log(LogLevel level, const std::string& fmt)
		{
			Logger::Get().EnqueueLog(fmt, level);
		}
	};
}