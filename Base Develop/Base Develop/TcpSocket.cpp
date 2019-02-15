#include "TcpSocket.h"
#include <memory>


TcpSocket::TcpSocket(ADDRESS_FAMILY f)
	:Socket(f)
{
}


TcpSocket::~TcpSocket()
{
}


bool TcpSocket::bind(const SocketAddress& addr)
{
	_socket = socket(_address_family, SOCK_STREAM, IPPROTO_TCP);

	if (_socket == INVALID_SOCKET)
	{
		_lastError = WSAGetLastError();
		return false;
	}


	int result = ::bind(_socket,
		addr.get_socketaddress(), addr.getSize());


	if (result == SOCKET_ERROR)
	{
		_lastError = WSAGetLastError();
		return false;
	}

	return true;
}

void TcpSocket::setNagle(bool bOption)
{
	int opt = bOption;

	setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY,
		(const char *)&opt, sizeof(int));
}

bool TcpSocket::listen(int backlog)
{
	int result = ::listen(_socket, backlog);

	if (result == SOCKET_ERROR)
	{
		_lastError = WSAGetLastError();
		return false;
	}

	return true;
}


std::shared_ptr<TcpSocket> TcpSocket::Accept(SocketAddress& address)
{
	int addressSize = address.getSize();

	SOCKET newSocket = ::accept(_socket,
		const_cast<sockaddr *>(address.get_socketaddress()),
		&addressSize);

	std::shared_ptr<TcpSocket> ret = std::make_shared<TcpSocket>(_address_family);

	ret->set_socket(newSocket);

	if (newSocket == INVALID_SOCKET)
		_lastError = WSAGetLastError();

	return ret;
}


std::shared_ptr<SocketAddress> TcpSocket::getAddress()
{
	SOCKADDR addr;

	int len = sizeof(addr);
	getpeername(_socket, (SOCKADDR *)&addr, &len);

	return std::make_shared<SocketAddress>(addr);
}

int TcpSocket::setNoDelay(bool toggle)
{
	return setsockopt(_socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&toggle, sizeof(bool));
}

int TcpSocket::Send(const void* inData, int inLen)
{
	int sentbyte = send(_socket, (const char*)(inData), inLen, 0);

	if (sentbyte >= 0)
		return sentbyte;

	_lastError = WSAGetLastError();
	return _lastError * -1;
}


int TcpSocket::Recv(void* inData, int inLen)
{
	int recvbytes = recv(_socket, static_cast<char *>(inData),
		inLen, 0);

	if (recvbytes >= 0)
		return recvbytes;

	_lastError = WSAGetLastError();
	return _lastError * -1;
}