#pragma once

#include "Socket.h"

class TcpSocket : public Socket
{
public:
	TcpSocket(ADDRESS_FAMILY f);
	virtual ~TcpSocket();

	void setNagle(bool bOption);

	bool connect(SocketAddress& serverAddress);

	virtual bool bind(const SocketAddress& addr);

	bool listen(int backlog);

	std::shared_ptr<TcpSocket> Accept();

	std::shared_ptr<SocketAddress> getAddress();

	int setNoDelay(bool toggle);

	// Not Use Socket IO Moudle Function
	int Send(const void* inData, int inLen);

	int Recv(void* inData, int inLen);
};

