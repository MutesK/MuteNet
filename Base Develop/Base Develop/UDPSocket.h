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

