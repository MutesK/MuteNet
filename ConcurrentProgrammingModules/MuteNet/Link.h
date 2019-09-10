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
		TcpSocket					_socket;
		ConnectPoint				_EndPoint;
		
		std::unique_ptr<Util::CircularBuffer> _RecvQ;
		std::unique_ptr<Util::CircularBuffer> _SendQ;

		std::atomic_bool					  _isSend;
	public:
		Link();
		virtual ~Link();

		void RecvPost();
		void SendPost();

		GET_SET_ATTRIBUTE(TcpSocket&, socket);
		GET_SET_ATTRIBUTE(ConnectPoint&, EndPoint);
		GET_SET_ATTRIBUTE(std::unique_ptr<Util::CircularBuffer>&, RecvQ);
		GET_SET_ATTRIBUTE(std::unique_ptr<Util::CircularBuffer>&, SendQ);
		GET_SET_ATTRIBUTE(std::atomic_bool&, isSend);
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
		return _socket.socket_handle();
	}
}
