#pragma once

#include <string>
#include <sstream>

class StringFormat
{
	static const int BUFFER_LENGTH = 25000;
public:
	template<typename ...Args>
	static std::string SharpFormat(std::string str, Args&& ... argument);

	static std::string Format(char* buffer, ...);

	static std::string GetCurrentDate();
private:
	template <typename Type, typename ...Args>  // �⺻ �ڷ��� üũ �ʿ�
	static std::string ConvertToString(Type value, Args&& ...Args);
}; 


