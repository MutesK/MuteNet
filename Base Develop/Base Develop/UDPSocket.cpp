#include "stdafx.h"
#include "UDPSocket.h"
#include "SocketAddress.h"

UDPSocket::UDPSocket(SOCKET inSocket)
	:_sock(inSocket)
{
	_err = NO_ERROR;
}

UDPSocket::~UDPSocket()
{
}

int UDPSocket::Bind(const SocketAddress& IN address)
{
	_err = bind(_sock, &address.get_addr(), address.GetSize());

	if (_error == 0)
		return NO_ERROR;

	// Report Error
	_error = WSAGetLastError();
	return _error;
}

int UDPSocket::SendTo(const void* inData, size_t lensize, const SocketAddress& IN into)
{
	int byteSendsize = sendto(_sock, static_cast<const char*>(inData),
		0, static_cast<int>(lensize),
		&into.get_addr(), into.GetSize());

	if (byteSendsize >= 0)
		return byteSendsize;

	// Report Error
	_err = WSAGetLastError();
	return _err;
}

int UDPSocket::ReceiveFrom(void *inBuffer, int lensize, SocketAddress& OUT From)
{
	int addressLength = From.GetSize();
	int readByteCount = recvfrom(_sock, static_cast<char *>(inBuffer),
		lensize, 0, &From.get_addr(), &addressLength);

	if (readByteCount >= 0)
		return readByteCount;

	// Report Error
	_err = WSAGetLastError();
	return _err;

}

int UDPSocket::GetLastError()
{
	return _err;
}