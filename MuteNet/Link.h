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
	
	class Link final
	{
	private:
		std::unique_ptr<TcpSocket>				_socket;
		ConnectPoint							_endPoint;
		
		std::unique_ptr<Util::CircularBuffer> _RecvQ;
		std::unique_ptr<Util::CircularBuffer> _SendQ;
	public:
		Link();
		virtual ~Link();

		TcpSocket* get_socket();
		Util::CircularBuffer* get_RecvQ();
		Util::CircularBuffer* get_SendQ();

		GET_SET_ATTRIBUTE(ConnectPoint&, endPoint);
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

	inline TcpSocket* Link::get_socket()
	{
		return _socket.get();
	}

	inline Util::CircularBuffer* Link::get_RecvQ()
	{
		return _RecvQ.get();
	}
	inline Util::CircularBuffer* Link::get_SendQ()
	{
		return _SendQ.get();
	}
}
