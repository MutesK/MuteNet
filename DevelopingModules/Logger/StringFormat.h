#pragma once

#include <string>
#include <sstream>

class StringFormat
{

public:
	template<typename ...Args>
	static void Format(std::string& str, Args&& ... argument);

private:
	template <typename Type, typename ...Args>  // 기본 자료형 체크 필요
	static std::string TypeConvert(Type value, Args&& ...Args);
}; 


