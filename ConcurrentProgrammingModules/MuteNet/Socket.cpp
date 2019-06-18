
#include "SocketAddress.h"
#include "Socket.h"

namespace Network
{

	Socket::Socket(ADDRESS_FAMILY family)
		:_address_family(family), _lastError(0), _handle(INVALID_SOCKET)
	{
	}

	Socket::~Socket()
	{
		closesocket(_handle);
	}

	int Socket::SetIoMode(bool ioflag)
	{
		// POSIX와 WinSock 함수가 다르지만 일단 윈도우만 지원한다.
		const auto ret = ioctlsocket(_handle, FIONBIO, reinterpret_cast<u_long*>(&ioflag));

		if (SOCKET_ERROR == ret)
		{
			_lastError = WSAGetLastError();
			return _lastError;
		}

		return true;
	}

	int Socket::Shutdown(ShutdownBlockMode Mode)
	{
		const auto result = ::shutdown(_handle, static_cast<int>(Mode));

		if (SOCKET_ERROR == result)
		{
			_lastError = WSAGetLastError();
			return _lastError;
		}

		return true;
	}
}