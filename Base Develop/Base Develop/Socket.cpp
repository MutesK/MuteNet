#include "Socket.h"



Socket::Socket(ADDRESS_FAMILY f)
	:_address_family(f)
{
	_handle = INVALID_SOCKET;
}


Socket::~Socket()
{
	closesocket(_handle);
}

int Socket::getLastError()
{
	return _lastError;
}

int Socket::setRecvSocketBufferSize(int size)
{
	return setsockopt(_handle, SOL_SOCKET, SO_RCVBUF, (const char *)&size, sizeof(int));
}

int Socket::setSendSocketBufferSize(int size)
{
	return setsockopt(_handle, SOL_SOCKET, SO_SNDBUF, (const char *)&size, sizeof(int));
}

int Socket::setReUseAddress(bool toggle)
{
	return setsockopt(_handle, SOL_SOCKET, SO_REUSEADDR, (const char *)&toggle, sizeof(bool));
}

int Socket::setRecvCallLimitTime(DWORD time)
{
	return setsockopt(_handle, SOL_SOCKET, SO_RCVTIMEO, (const char *)&time, sizeof(DWORD));
}

int Socket::setSendCallLimitTime(DWORD time)
{
	return setsockopt(_handle, SOL_SOCKET, SO_SNDTIMEO, (const char *)&time, sizeof(DWORD));
}

int Socket::setUseKeepAlive(bool toggle)
{
	return setsockopt(_handle, SOL_SOCKET, SO_KEEPALIVE, (const char *)&toggle, sizeof(bool));
}

int Socket::setIOMode(bool ioflag)
{
	// POSIX와 WinSock 함수가 다르지만 일단 윈도우만 지원한다.
	int ret = ioctlsocket(_handle, FIONBIO, (u_long *)&ioflag);

	if (SOCKET_ERROR == ret)
	{
		_lastError = WSAGetLastError();
		return _lastError;
	}

	return true;
}

int Socket::shutdown(ShutdownBlockMode Mode)
{
	int result = ::shutdown(_handle, static_cast<int>(Mode));

	if (SOCKET_ERROR == result)
	{
		_lastError = WSAGetLastError();
		return _lastError;
	}

	return true;
}