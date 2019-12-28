#pragma once

#pragma warning(disable: 4717)  // StringFormatter Recursive Unpacking

#include "foundation.h"
#include <atlstr.h>

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

		template <typename... Args>
		static std::string FormatUsingSpecfiler(const std::string fmt, Args... args);

		template <typename Type>
		static std::string ToString(const Type type);

		static std::wstring CA2W(const std::string& MultiByte, uint32_t CodePage);

		static std::string CW2A(const std::wstring& Unicode, uint32_t CodePage);
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
				sprintf_s(bracket, 100, "{%llu}", index);

				const auto findIndex = ret.find(bracket, strlen(bracket));
				if (findIndex == std::string::npos)
					break;

				ret.replace(findIndex, strlen(bracket), formatter._ArgumentList[index]);
			}
		}

		return ret;
	}

	template<typename ...Args>
	inline std::string StringHelper::FormatUsingSpecfiler(const std::string fmt, Args ...args)
	{
		char buffer[10000]{0};

		sprintf_s(buffer, fmt.c_str(), std::forward<Args>(args)...);
		return std::string(buffer);
	}

	template<typename Type>
	inline std::string StringHelper::ToString(const Type type)
	{
		static_assert(std::is_arithmetic_v<type>, "ToString is must Arithmertic Type.");

		return std::to_string(type);
	}


	inline std::wstring StringHelper::CA2W(const std::string& MultiByte, uint32_t CodePage)
	{
		return std::wstring(ATL::CA2W(MultiByte.c_str(), CodePage));
	}

	inline std::string StringHelper::CW2A(const std::wstring& Unicode, uint32_t CodePage)
	{
		return std::string(ATL::CW2A(Unicode.c_str(), CodePage));
	}

}