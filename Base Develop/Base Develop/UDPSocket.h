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
	~UDPSocket();

	// No Socket I/O Model Function
	int Bind(const SocketAddress& IN address);
	int SendTo(const void* inData, size_t lensize, const SocketAddress& IN into);
	int ReceiveFrom(void *inBuffer, int lensize, SocketAddress&  OUT From);

	int GetLastError();

	GET_SET_ATTRIBUTE(SOCKET, sock);
	GET_CONST_ATTRIBUTE(int, err);

	friend class SocketUtil;
private:
	UDPSocket(SOCKET inSocket);
private:
	SOCKET _sock;
	int	   _err;
};

