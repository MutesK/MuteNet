#pragma once
#include "Socket.h"
class TcpSocket : public Socket
{
protected:
	TcpSocket(ADDRESS_FAMILY f);
public:
	virtual ~TcpSocket();

	void setNagle(bool bOption);

	virtual bool bind(const SocketAddress& addr);

	bool listen(int backlog);

	std::shared_ptr<TcpSocket> Accept(SocketAddress& address);

	std::shared_ptr<SocketAddress> getAddress();

	int setNoDelay(bool toggle);

	// Not Use Socket IO Moudle Function
	int Send(const void* inData, int inLen);

	int Recv(void* inData, int inLen);

};

