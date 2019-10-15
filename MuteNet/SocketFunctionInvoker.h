#pragma once

#include "framework.h"

namespace MuteNet
{
	class SocketDelegateInvoker // Invoker
	{
	public:
		template <class Func, typename ...Args >
		static inline int Invoke(Func&& func, Args ...args)
		{
			int ret = func(std::forward<Args>(args)...);
			if (ret == SOCKET_ERROR)
			{
				auto getLastError = WSAGetLastError();

				if (getLastError == WSA_IO_PENDING)
					return ERROR_SUCCESS;

				return getLastError;
			}

			return ERROR_SUCCESS;
		}
	};

	inline std::string ErrorString(int ErrorCode)
	{
		// Note gai_strerror is not threadsafe on windows

		char ErrorStr[GAI_STRERROR_BUFFER_SIZE + 1];

		int MsgLen = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			nullptr,
			ErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			ErrorStr,
			sizeof(ErrorStr) - 1,
			nullptr
		);

		return std::string(ErrorStr, MsgLen);
	}
}