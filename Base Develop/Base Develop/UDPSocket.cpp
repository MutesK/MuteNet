#include "stdafx.h"
#include "UDPSocket.h"
#include "SocketAddress.h"

UDPSocket::UDPSocket(SOCKET inSocket)
	:_socket(inSocket)
{
}

UDPSocket::~UDPSocket()
{
}

int UDPSocket::Bind(const SocketAddress& IN address)
{
	_error = bind(_socket, &address.get_addr(), address.GetSize());

	if (_error == 0)
		return NO_ERROR;

	// Report Error
}

int UDPSocket::SendTo(const void* inData, size_t lensize, const SocketAddress& IN into)
{
	int byteSendsize = sendto(_socket, static_cast<const char*>(inData),
		0, static_cast<int>(lensize),
		&into.get_addr(), into.GetSize());

	if (byteSendsize >= 0)
		return byteSendsize;

	// Report Error
}

int UDPSocket::ReceiveFrom(void *inBuffer, int lensize, SocketAddress& OUT From)
{
	int addressLength = From.GetSize();
	int readByteCount = recvfrom(_socket, static_cast<char *>(inBuffer),
		lensize, 0, &From.get_addr(), &addressLength);

	if (readByteCount >= 0)
		return readByteCount;

	// Report Error

}

int UDPSocket::GetLastError()
{
	return _error;
}