#pragma once

#include "StringHelper.h"

namespace Util
{
	class Exception : public std::exception
	{
	private:
		std::string _file;
		std::string _function;
		std::string errorMessage;
		uint32_t _errorCode;
		uint64_t _line;
	public:
		Exception(uint32_t errorCode, const char* ErrorMessage, const char* pFile,
			const uint64_t& line, const char *pFunction) noexcept
			:_errorCode(errorCode), errorMessage(ErrorMessage), _file(pFile),
			_line(line), _function(pFunction)
		{	
		}

		inline virtual std::string what()
		{
			return StringHelper::Format("Function:[{0}] Error:[{1}:{2}] File:[{3}:{4}]",
				_function, _errorCode, errorMessage, _file, _line);
		}
	};

}
