#pragma once

#include "Socket.h"

class UdpSocket : public Socket
{
public:
	UdpSocket(ADDRESS_FAMILY);
	virtual ~UdpSocket();

	virtual bool bind(const SocketAddress& address);

	int SendTo(const void* inData, int inLen, const SocketAddress& inTo);

	int ReceiveFrom(void *inBuffer, int inLen, SocketAddress& outFrom);

	friend class SocketUtil;

};

