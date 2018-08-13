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
	explicit UDPSocket(SOCKET inSocket);
	~UDPSocket();

	// No Socket I/O Model Function -> It purpose Test For UDP Socket
	int Bind(const SocketAddress& IN address);
	int SendTo(const void* inData, size_t lensize, const SocketAddress& IN into);
	int ReceiveFrom(void *inBuffer, int lensize, SocketAddress&  OUT From);

	int GetLastError();
private:
	SOCKET _socket;
	int	   _error;
};

