#pragma once

#include <exception>
#include <string>

namespace Util
{
	class Exception : public std::exception
	{
	public:
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
