#pragma once

#include "foundation.h"

namespace Util
{
	class StringHelper
	{
	private:
		class StringFormater
		{
		public:
			std::vector<std::string> _ArgumentList;
		
			template <typename ...Args>
			void Format(Args&& ...argument)
			{
				Format(argument...);
			}

			template <typename Type, typename ...Args>
			void Format(Type type, Args&& ...argument)
			{
				Format(type);

				Format(argument...);
			}

			template <typename Type>
			void Format(Type type)
			{
				std::stringstream stream;
				stream << type;

				_ArgumentList.push_back(stream.str());
			}

			template <>
			void Format(const char* ptr)
			{
				_ArgumentList.emplace_back(std::string(ptr));
			}

			template <>
			void Format(const std::string str)
			{
				_ArgumentList.emplace_back(str);
			}
		};
	public:
		template <typename... Args>
		static std::string Format(const std::string fmt, Args... args);
	};


	template<typename ...Args>
	inline std::string StringHelper::Format(const std::string fmt, Args ...args)
	{
		StringFormater formatter;

		std::string ret = fmt;

		if (sizeof...(args) > 0)
		{
			formatter.Format(std::forward<Args>(args)...);
			char bracket[100]{ 0 };

			for (std::size_t index = 0; index < formatter._ArgumentList.size(); index++)
			{
				sprintf_s(bracket, 100, "{%I64u}", index);

				const auto findIndex = ret.find(bracket, strlen(bracket));
				if (index == std::string::npos)
					break;

				ret.replace(findIndex, strlen(bracket), formatter._ArgumentList[index]);
			}
		}

		return ret;
	}
}