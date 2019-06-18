#pragma once

#include "../RefCount/RefCount.h"
#include "Socket.h"
#include "TcpSocket.h"
#include "EndPoint.h"

namespace Network
{
	class Link;
	typedef RefCountPtr<Link> LinkPtr;

	class Link final
	{
	public:
		typedef Link* Handle;
	private:
		std::shared_ptr<TcpSocket> _socket;
		std::atomic_int _refCount;

		OVERLAPPED _SendOverlapped;
		OVERLAPPED _RecvOverlapped;

		// Buffer(Queue)

	public:
		Link();
		~Link();

		// User Request Function
		// Send Packet Request

		static EndPoint* GetEndPoint(Handle handle);
		static void ForceDisconnect(Handle handle);
	private:
		friend class Acceptor;
		friend class IOEngine;
		friend class IOService;

	public:
		void AddRef();
		void Release();

		void* native_handle();
		// Inner Function To Manangement Session
		// Buffer Management
		// IOCP Request
	};
}
