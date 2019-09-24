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
	class IOContext;
	
	class Link final : std::enable_shared_from_this<Link>
	{
	private:
		std::shared_ptr<TcpSocket>	_socket;
		ConnectPoint&				_endPoint;
		
		std::shared_ptr<Util::CircularBuffer> _RecvQ;
		std::shared_ptr<Util::CircularBuffer> _SendQ;

		std::atomic_bool					  _isSend;
	public:
		Link(std::shared_ptr<TcpSocket>& socket, ConnectPoint&);
		virtual ~Link();

		void RecvPost();
		void SendPost();
		void SendPacket(const std::shared_ptr<Util::OutputMemoryStream>& Packet);

		GET_SET_ATTRIBUTE(bool, isSend);
		GET_SET_ATTRIBUTE(std::shared_ptr<TcpSocket>&, socket);
		GET_SET_ATTRIBUTE(ConnectPoint&, endPoint);
		GET_SET_ATTRIBUTE(std::shared_ptr<Util::CircularBuffer>&, RecvQ);
		GET_SET_ATTRIBUTE(std::shared_ptr<Util::CircularBuffer>&, SendQ);
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
		return _socket->socket_handle();
	}
}
