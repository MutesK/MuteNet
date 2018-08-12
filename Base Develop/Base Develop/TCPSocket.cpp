#include "stdafx.h"
#include "TCPSocket.h"
#include "SocketAddress.h"

TCPSocket::TCPSocket(SOCKET s)
	:_sock(s)
{
	_err = NO_ERROR;
}


TCPSocket::~TCPSocket()
{
}

int TCPSocket::Connect(const SocketAddress& IN Address)
{
	int err = connect(_sock, &Address.get_addr(), Address.GetSize());
	if (err >= 0)
		return NO_ERROR;

	// Report Error
	_err = WSAGetLastError();
	return _err;
}

int TCPSocket::Bind(const SocketAddress& IN Address)
{
	_err = bind(_sock, &Address.get_addr(), Address.GetSize());

	if (_err == 0)
		return NO_ERROR;

	// Report Error
	_err = WSAGetLastError();
	return _err;
}

int TCPSocket::Listen(int backlog)
{
	int err = listen(_sock, backlog);
	if (err >= 0)
		return NO_ERROR;

	// Report Error
	_err = WSAGetLastError();
	return _err;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress& IN Fromaddress)
{
	int length = Fromaddress.GetSize();
	SOCKET newSocket = accept(_sock, &Fromaddress.get_addr(), &length);

	if (newSocket != INVALID_SOCKET)
		return std::make_shared<TCPSocket>(newSocket);

	_err = WSAGetLastError();
	return std::make_shared<TCPSocket>(INVALID_SOCKET);
}

int TCPSocket::Send(const void* inData, int Len)
{
	int byteSendCount = send(_sock, static_cast<const char *>(inData), Len, 0);

	if (byteSendCount >= 0)
		return byteSendCount;

	_err = WSAGetLastError();
	return _err;
}

int TCPSocket::Recv(void *inBuffer, int Len)
{
	int byteRecvCount = recv(_sock, static_cast<char *>(inBuffer), Len, 0);

	if (byteRecvCount >= 0)
		return byteRecvCount;

	_err = WSAGetLastError();
	return _err;
}