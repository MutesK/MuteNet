#pragma once

#include <exception>
#include <string>

namespace Util
{
	class Exception final : public std::exception
	{
		explicit Exception(char const* const message) noexcept
			:std::exception(message)
		{	
		}

		explicit Exception(const std::string message) noexcept
			:std::exception(message.c_str())
		{
		}
	};
}
