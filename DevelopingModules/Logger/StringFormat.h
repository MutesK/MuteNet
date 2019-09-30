#pragma once

#include <string>
#include <sstream>

class StringFormat
{

public:
	template<typename ...Args>
	static void Format(std::string& str, Args&& ... argument);

private:
	template <typename Type, typename ...Args>  // �⺻ �ڷ��� üũ �ʿ�
	static std::string TypeConvert(Type value, Args&& ...Args);
}; 


