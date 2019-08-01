#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "../MemoryStream/MemoryStream.h"

namespace Network
{
	class AddReadIO;
	class Link;

	class Link final : std::enable_shared_from_this<Link>
	{
	public:
		typedef Link* Handle;
	private:
		std::shared_ptr<TcpSocket> _socket;
		std::atomic_int _refCount;

		ConnectPoint _RemotePoint;
		ConnectPoint _EndPoint;
	public:
		Link(const std::shared_ptr<TcpSocket>& socket);
		Link(const TcpSocket* socket);

		~Link();

	private:
		friend class Acceptor;
		friend class IOService;
		friend class AddReadIO;
	public:
		// Inner Function To Manangement Session
		// Buffer Management
		// IOCP Request
	};
}
