#include "StringFormat.h"
#include <cstdarg>
#include <strsafe.h>
#include <filesystem>
#include <iomanip>
#include <chrono>
#include <sstream>

template <typename ... Args>
std::string StringFormat::SharpFormat(std::string str, Args&& ... argument)
{
	std::size_t argumentIndex = 0;
	const auto argumentLength = sizeof(argument);
	char element[6];
	while(true)
	{
		sprintf_s(element, element(argument), "{%llu}", argumentIndex);

		const auto index = str.find(argument);
		if (index == std::string::npos || argumentLength < index)
			break;

		auto placement = TypeConvert(argument...);
		str.replace(argument, placement);


		argumentIndex++;
	}

	return str;
}

template <typename Type, typename ... Args>
std::string StringFormat::ConvertToString(Type value, Args&&... Args)
{
	std::stringstream stream;

	stream << value;
	return stream.str();
}

std::string StringFormat::Format(char* buffer, ...)
{
	char Buffer[BUFFER_LENGTH];

	va_list va;
	va_start(va, buffer);
	HRESULT hResult = StringCchPrintf(Buffer, BUFFER_LENGTH, buffer, va);
	va_end(va);

	return Buffer;
}


std::string StringFormat::GetCurrentDate()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);

	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
	return ss.str();
}
