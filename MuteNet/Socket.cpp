
#include "pch.h"
#include "Socket.h"
#include "ConnectPoint.h"
namespace MuteNet
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
		return SocketDelgateInvoker::Invoke(ioctlsocket, _handle, FIONBIO, reinterpret_cast<u_long*>(&ioflag));
	}

	int Socket::GetAddress(ConnectPoint& Point) const
	{
		SOCKADDR_IN Address;
		int addrlen = sizeof(SOCKADDR_IN);

		return SocketDelgateInvoker::Invoke(getpeername, _handle, reinterpret_cast<SOCKADDR*>(&Address), &addrlen);
	}

	int Socket::Shutdown(ShutdownBlockMode Mode)
	{
		return SocketDelgateInvoker::Invoke(shutdown, _handle, static_cast<int>(Mode));
	}

}
