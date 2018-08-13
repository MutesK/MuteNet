#pragma once

/*
	 소멸자를 통해 소켓이 누수되지않게 한다.

	 앞으로의 계획.

	 IOCP 모듈에 Plug-in하면 자동으로 셋팅되게 하고자한다.
*/

class SocketAddress;
class UDPSocket
{
public:
	UDPSocket(SOCKET inSocket);
	~UDPSocket();

	// No Socket I/O Model Function -> It purpose Test For UDP Socket
	int Bind(const SocketAddress& IN address);
	int SendTo(const void* inData, size_t lensize, const SocketAddress& IN into);
	int ReceiveFrom(void *inBuffer, int lensize, SocketAddress&  OUT From);

	int GetLastError();

	GET_SET_ATTRIBUTE(SOCKET, sock);
	GET_CONST_ATTRIBUTE(int, err);

private:
	friend class SocketUtil;
	SOCKET _sock;
	int	   _err;
};

