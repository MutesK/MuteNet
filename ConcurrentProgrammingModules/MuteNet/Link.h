#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"

namespace Network
{
	typedef Link* LinkPtr;

	class Link final
	{
	private:
		TcpSocket	 _Socket;

		ConnectPoint _RemotePoint;
		ConnectPoint _EndPoint;
	private:
		Link();
		~Link();

		SOCKET		socket_handle() const;

		friend class LinkManager;
		friend class Acceptor;
		friend class IOService;
	};

	inline SOCKET Link::socket_handle() const 
	{
		return _Socket.socket_handle();
	}
}
