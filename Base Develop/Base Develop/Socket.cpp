#include "Socket.h"



Socket::Socket(ADDRESS_FAMILY f)
	:_address_family(f)
{
	_socket = INVALID_SOCKET;
}


Socket::~Socket()
{
	closesocket(_socket);
}

int Socket::getLastError()
{
	return _lastError;
}

int Socket::setRecvSocketBufferSize(int size)
{

}

int Socket::setSendSocketBufferSize(int size)
{

}

int Socket::setReUseAddress(bool toggle)
{

}

int Socket::setRecvCallLimitTime(DWORD time)
{

}

int Socket::setSendCallLimitTime(DWORD time)
{

}

int Socket::setUseKeepAlive(bool toggle)
{

}

int Socket::setIOMode(bool ioflag)
{
	// POSIX와 WinSock 함수가 다르지만 일단 윈도우만 지원한다.
	int ret = ioctlsocket(_socket, FIONBIO, (u_long *)&ioflag);

	if (SOCKET_ERROR == ret)
	{
		_lastError = WSAGetLastError();
		return -_lastError;
	}

	return true;
}