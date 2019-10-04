#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "IOContext.h"

namespace Network
{
	class IOContext;
	
	using LinkPtr = Link *;
	class Link final : public Util::ReferenceCounter
	{
	private:
		TcpSocket								_socket;
		ConnectPoint							_endPoint;
		
		// Buffer Pool °í¹Î
		Util::CircularBuffer					_RecvQ;
		Util::CircularBuffer					_SendQ;

		SendContext							  _SendContext;
		RecvContext							  _RecvContext;
	public:
		Link();
		virtual ~Link();

		void SendPost();
		void RecvPost();

		void SendPacket(const std::shared_ptr<Util::OutputMemoryStream>& Packet);

		GET_SET_ATTRIBUTE(ConnectPoint&, endPoint);
	private:
		void SendCompleteIO(DWORD TransfferedBytes, void* CompletionKey);
		void RecvCompleteIO(DWORD TransfferedBytes, void* CompletionKey);

		virtual void decRefCount();

		GET_SET_ATTRIBUTE(TcpSocket&, socket);
		GET_SET_ATTRIBUTE(Util::CircularBuffer&, RecvQ);
		GET_SET_ATTRIBUTE(Util::CircularBuffer&, SendQ);
	private:
		friend class LinkManager;
		friend class Acceptor;
		friend class Connector;
		friend class IOService;
		friend class IOContext;
	};

}
