#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "IOContext.h"

namespace Util
{
	class CircularBuffer;
};

namespace Network
{
	struct IOContext;
	
	class Link final : std::enable_shared_from_this<Link>
	{
	private:
		TcpSocket					_Socket;
		ConnectPoint				_EndPoint;
		
		std::unique_ptr<Util::CircularBuffer> _RecvQ;
		std::unique_ptr<Util::CircularBuffer> _SendQ;
	public:
		Link();
		virtual ~Link();

		void RecvPost();
		void SendPost();
	private:
		SOCKET		socket_handle() const;

		friend class LinkManager;
		friend class Acceptor;
		friend class Connector;
		friend class IOService;
		friend class IOContext;
	};

	inline SOCKET Link::socket_handle() const 
	{
		return _Socket.socket_handle();
	}
}
