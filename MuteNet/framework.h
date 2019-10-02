#pragma once

#pragma warning(disable: 4267)
#pragma warning(disable: 6031)

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")


#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#include "../Foundation/foundation.h"
#include "../Foundation/ASyncQueue.h"
#include "../Foundation/InputMemoryStream.h"
#include "../Foundation/OutputMemoryStream.h"
#include "../Foundation/CircularBuffer.h"

static const DWORD AddressLength = sizeof(SOCKADDR_IN) + 16;


class SocketDelgateInvoker // Invoker
{
public:
	template <class Func,  typename ...Args >
	static bool Invoke(Func&& func, Args ...args)
	{
		int ret = func((args)...);
		if (ret == SOCKET_ERROR)
		{
			auto getLastError = WSAGetLastError();

			if (getLastError == WSA_IO_PENDING)
				return true;

			std::cout << "Socket Error : " << getLastError << std::endl;

			return false;
		}

		return true;
	}
};


