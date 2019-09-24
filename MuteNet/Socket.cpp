
#include "pch.h"
#include "Socket.h"
#include "ConnectPoint.h"

namespace Network
{

	Socket::Socket(ADDRESS_FAMILY family)
		:_address_family(family), _lastError(0), _handle(INVALID_SOCKET)
	{
	}

	Socket::Socket(SOCKET socket)
		: _address_family(AF_INET), _handle(INVALID_SOCKET), _lastError(0)
	{
	}

	Socket::~Socket()
	{
		closesocket(_handle);
	}

	int Socket::SetIoMode(bool ioflag)
	{
		// POSIX�� WinSock �Լ��� �ٸ����� �ϴ� �����츸 �����Ѵ�.
		const auto ret = ioctlsocket(_handle, FIONBIO, reinterpret_cast<u_long*>(&ioflag));

		if (SOCKET_ERROR == ret)
		{
			_lastError = WSAGetLastError();
			return _lastError;
		}

		return true;
	}

	int Socket::GetAddress(ConnectPoint& Point) const
	{
		SOCKADDR_IN Address;
		int addrlen = sizeof(SOCKADDR_IN);

		if (SOCKET_ERROR == getpeername(_handle, reinterpret_cast<SOCKADDR*>(&Address), &addrlen))
			return false;

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