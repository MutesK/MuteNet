#include "MuteNetFoundation.h"
#include "SocketAddress.h"
#include "Socket.h"
#include "TcpSocket.h"


TcpSocket::TcpSocket(ADDRESS_FAMILY f)
	:Socket(f)
{
	_handle = socket(_address_family, SOCK_STREAM, IPPROTO_TCP);
}

bool TcpSocket::bind(const std::shared_ptr<SocketAddress>& address)
{
	if (_handle == INVALID_SOCKET)
	{
		_lastError = WSAGetLastError();
		return false;
	}

	int result = ::bind(_handle,
		address->get_socketaddress(), address->getSize());

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

	setsockopt(_handle, IPPROTO_TCP, TCP_NODELAY,
		(const char *)&opt, sizeof(int));
}

bool TcpSocket::connect(SocketAddress& serverAddress)
{
	int result = ::connect(_handle, serverAddress.get_socketaddress(),
		serverAddress.getSize());

	if (result == SOCKET_ERROR)
	{
		_lastError = WSAGetLastError();
		_handle = INVALID_SOCKET;
		return false;
	}

	return true;
}

bool TcpSocket::listen(int backlog)
{
	int result = ::listen(_handle, backlog);

	if (result == SOCKET_ERROR)
	{
		_lastError = WSAGetLastError();
		_handle = INVALID_SOCKET;
		return false;
	}

	return true;
}


std::shared_ptr<TcpSocket> TcpSocket::Accept()
{
	SOCKADDR_IN ClientAddress;

	int addressSize = sizeof(ClientAddress);

	SOCKET newSocket = ::accept(_handle,
		reinterpret_cast<sockaddr *>(&ClientAddress),
		&addressSize);

	std::shared_ptr<TcpSocket> ret = std::make_shared<TcpSocket>(_address_family);

	ret->set_handle(newSocket);

	if (newSocket == INVALID_SOCKET)
		_lastError = WSAGetLastError();

	return ret;
}


std::shared_ptr<SocketAddress> TcpSocket::getAddress()
{
	SOCKADDR addr;

	int len = sizeof(addr);
	getpeername(_handle, (SOCKADDR *)&addr, &len);

	return std::make_shared<SocketAddress>(addr);
}

int TcpSocket::setNoDelay(bool toggle)
{
	return setsockopt(_handle, IPPROTO_TCP, TCP_NODELAY, (const char *)&toggle, sizeof(bool));
}

int TcpSocket::Send(const void* inData, int inLen)
{
	int sentbyte = send(_handle, (const char*)(inData), inLen, 0);

	if (sentbyte >= 0)
		return sentbyte;

	_lastError = WSAGetLastError();
	return 0;
}


int TcpSocket::Recv(void* inData, int inLen)
{
	int recvbytes = recv(_handle, static_cast<char *>(inData),
		inLen, 0);

	if (recvbytes >= 0)
		return recvbytes;

	_lastError = WSAGetLastError();
	return 0;
}

int TcpSocket::OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO)
{
	DWORD RecvSize = 0;
	DWORD Flag = 0;

	int result = WSARecv(_handle, pBufArr, Arrlen, &RecvSize, &Flag, (LPOVERLAPPED)OverlappedIO, nullptr);

	int errorCode = WSAGetLastError();

	if ((result != SOCKET_ERROR) || (result == SOCKET_ERROR && errorCode == WSA_IO_PENDING))
		return result;

	_lastError = errorCode;
	return -1;
}

int TcpSocket::OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO)
{
	DWORD SendSize = 0;
	DWORD Flag = 0;

	int result = WSASend(_handle, pBufArr, Arrlen, &SendSize, Flag, (LPOVERLAPPED)OverlappedIO, nullptr);

	int errorCode = WSAGetLastError();

	if ((result != SOCKET_ERROR) || (result == SOCKET_ERROR && errorCode == WSA_IO_PENDING))
		return result;

	_lastError = errorCode;
	return -1;
}