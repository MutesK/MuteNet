#include "StringFormat.h"

template <typename ... Args>
void StringFormat::Format(std::string& str, Args&& ... argument)
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
}

template <typename Type, typename ... Args>
std::string StringFormat::TypeConvert(Type value, Args&&... Args)
{
	std::stringstream stream;

	stream << value;
	return stream.str();
}
