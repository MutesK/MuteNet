#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "IOContext.h"

namespace Util
{
	class OutputByteStream;
};

namespace Network
{
	struct IOContext;
	
	class Link final : std::enable_shared_from_this<Link>
	{
	private:
		TcpSocket					_Socket;
		ConnectPoint				_EndPoint;

		ContextCallback				_CallBack;
	public:
		Link();
		virtual ~Link();

		void RecvPost();
		void SendPost();

	private:
		void IOCompletion(IOContext* pContext, DWORD TransferredBytes, void* CompletionKey);

		SOCKET		socket_handle() const;

		friend class LinkManager;
		friend class Acceptor;
		friend class Connector;
		friend class IOService;
	};

	inline SOCKET Link::socket_handle() const 
	{
		return _Socket.socket_handle();
	}
}
