#pragma once
#include "Socket.h"
class UdpSocket : public Socket
{
protected:
	UdpSocket(ADDRESS_FAMILY);
public:
	virtual ~UdpSocket();

	virtual bool bind(const SocketAddress& address);

	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);

	int ReceiveFrom(void *inBuffer, int inLen, SocketAddress& outFrom);

};

