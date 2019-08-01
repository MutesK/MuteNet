#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "../MemoryStream/MemoryStream.h"

namespace Network
{
	typedef Link* LinkPtr;

	class Link final : std::enable_shared_from_this<Link>
	{
	private:
		TcpSocket	 _Socket;

		ConnectPoint _RemotePoint;
		ConnectPoint _EndPoint;
	private:
		Link();
		~Link();

		SOCKET		socket_handle();

		friend class LinkManager;
		friend class Acceptor;
		friend class IOService;
	};
}
