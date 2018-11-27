#pragma once

#include "IOCallback.h"
#include "OutputMemoryStream.h"

class SocketAddress;
class TCPSocket
{
public:
	TCPSocket(SOCKET s);
	~TCPSocket();

// TCP �⺻ �Լ�
	int Connect(const SocketAddress& IN Address);
	int Bind(const SocketAddress& IN Address);
	int Listen(int backlog = 32);
	std::shared_ptr<TCPSocket> Accept(SocketAddress& IN Fromaddress);

	int Send(const void* inData, int Len);
	int Recv(void *inBuffer, int Len);

	// IOCP function
	int Recv(OutputMemoryStream& stream, IOCallback& recvIO);

	GET_SET_ATTRIBUTE(SOCKET, sock);
	GET_CONST_ATTRIBUTE(int, err);

private:
	friend class SocketUtil;

	SOCKET		_sock;
	IOCallback  _callback;

	int	   _err;
};

