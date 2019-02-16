#pragma once

/*
	이 클래스를 한번더  오버랩해서 레이턴시, 지터링, 신뢰성을 추가할 예정.
*/
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

