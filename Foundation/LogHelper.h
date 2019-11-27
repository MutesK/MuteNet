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
			const std::string result = StringHelper::Format(format, std::forward<Args>(args)...);

			Logger::Get().EnqueueLog(result, level);
		}
	};
}