#pragma once

#include <sql.h>
#include <sqlext.h>
#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace ODBC
{
	
	template <typename T>
	using is_string = std::integral_constant<bool,
		std::is_same_v<typename std::decay<T>::type, std::string> ||
		std::is_same_v<typename std::decay<T>::type, std::string>>;

	template <typename T>
	using is_character = std::integral_constant<bool,
		std::is_same_v<typename std::decay<T>::type, std::string::value_type> ||
		std::is_same_v<typename std::decay<T>::type, std::string::value_type>>;

	inline bool Success(RETCODE code)
	{
		if (code == SQL_SUCCESS || code == SQL_SUCCESS_WITH_INFO)
			return true;

		return false;
	}

	inline std::string return_code(RETCODE rc)
	{
		switch (rc)
		{
		case SQL_SUCCESS:
			return "SQL_SUCCESS";
		case SQL_SUCCESS_WITH_INFO:
			return "SQL_SUCCESS_WITH_INFO";
		case SQL_ERROR:
			return "SQL_ERROR";
		case SQL_INVALID_HANDLE:
			return "SQL_INVALID_HANDLE";
		case SQL_NO_DATA:
			return "SQL_NO_DATA";
		case SQL_NEED_DATA:
			return "SQL_NEED_DATA";
		case SQL_STILL_EXECUTING:
			return "SQL_STILL_EXECUTING";
		default:
			return "FAILED";
		}
	}

#define CHECK_AND_EXCEPT(RETURN)																		\
		if(!Success(RETURN))																			\
		{																								\
			std::stringstream out;																		\
			out << __FUNCTION__ << " Error : " << return_code(RETURN);									\
			throw std::runtime_error(out.str());														\
		}

}