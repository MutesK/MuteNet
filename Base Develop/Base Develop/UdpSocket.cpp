#include "UdpSocket.h"



UdpSocket::UdpSocket(ADDRESS_FAMILY f)
	:Socket(f)
{
	_socket = socket(_address_family, SOCK_DGRAM, IPPROTO_UDP);
}


UdpSocket::~UdpSocket()
{
}

bool UdpSocket::bind(const SocketAddress& address)
{

	if (_socket == INVALID_SOCKET)
	{
		_lastError = WSAGetLastError();
		return false;
	}

	int result = ::bind(_socket,
		address.get_socketaddress(), address.getSize());

	if (result == SOCKET_ERROR)
	{
		_lastError = WSAGetLastError();
		return false;
	}

	return true;
}

int UdpSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
	int byte = sendto(_socket, (const char *)inData, inLen, 0, inTo.get_socketaddress(),
		inTo.getSize());

	if (byte >= 0)
		return byte;

	_lastError = WSAGetLastError();
	return _lastError * -1;
}

int UdpSocket::ReceiveFrom(void *inBuffer, int inLen, SocketAddress& outFrom)
{
	int length = outFrom.getSize();

	int recv = recvfrom(_socket, (char*)inBuffer, inLen, 0, 
		const_cast<sockaddr *>(outFrom.get_socketaddress()),
		&length);

	if (recv >= 0)
		return recv;

	_lastError = WSAGetLastError();
	return _lastError * -1;
}