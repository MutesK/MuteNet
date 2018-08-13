#pragma once

/*
	 �Ҹ��ڸ� ���� ������ ���������ʰ� �Ѵ�.

	 �������� ��ȹ.

	 IOCP ��⿡ Plug-in�ϸ� �ڵ����� ���õǰ� �ϰ����Ѵ�.
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

