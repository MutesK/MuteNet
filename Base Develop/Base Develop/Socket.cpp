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
	// POSIX�� WinSock �Լ��� �ٸ����� �ϴ� �����츸 �����Ѵ�.
	int ret = ioctlsocket(_socket, FIONBIO, (u_long *)&ioflag);

	if (SOCKET_ERROR == ret)
	{
		_lastError = WSAGetLastError();
		return -_lastError;
	}

	return true;
}