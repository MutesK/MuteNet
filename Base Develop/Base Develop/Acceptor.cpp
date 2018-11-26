#include "stdafx.h"
#include "Acceptor.h"
#include "SocketUtil.h"
#include "SocketAddress.h"
#include "TCPSocket.h"
#include "UDPSocket.h"
#include "CSession.h"

Acceptor::Acceptor(const std::string& IpAddress)
{
	_ipAddress = IpAddress;
}


Acceptor::~Acceptor()
{
}

void Acceptor::DoWork()
{
	while (!_threadstoprequest)
	{
		auto client = SocketUtil::CreateTCPSocket(INET);
		auto clientAddress = SocketUtil::CreateIPv4FromString("");

		std::shared_ptr<CSession> Session = std::make_shared<CSession>(client, clientAddress);

		client->Accept(*clientAddress);

		if (client->get_sock() == INVALID_SOCKET)
		{
			continue;
		}

		if (_isUseNagle)
		{
			int opt = 1;
			setsockopt(client->get_sock(), IPPROTO_TCP, TCP_NODELAY,
				reinterpret_cast<const char *>(opt), sizeof(int));
		}

		OnAccept(Session);
	}
}