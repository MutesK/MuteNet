#pragma once

#include "../RefCount/RefCount.h"
#include "Socket.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"
#include "../MemoryStream/MemoryStream.h"

namespace Network
{
	class AddReadIO;
	class Link;
	typedef RefCountPtr<Link> LinkPtr;

	class Link final
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
		void AddRef();
		void Release();

		void SetRemotePoint(SOCKADDR* Remote);
		void SetEndPoint(SOCKADDR* End);


		// Inner Function To Manangement Session
		// Buffer Management
		// IOCP Request
	};
	inline void Link::SetRemotePoint(SOCKADDR* Remote)
	{
		_RemotePoint.SetConnectPoint(*Remote);
	}

	inline void Link::SetEndPoint(SOCKADDR* End)
	{
		_EndPoint.SetConnectPoint(*End);
	}
}
