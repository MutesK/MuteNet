#pragma once

#include "../RefCount/RefCount.h"
#include "Socket.h"
#include "TcpSocket.h"

namespace Network
{
	class Link final
	{
	private:
		std::shared_ptr<TcpSocket> _socket;
		// EndPoint
		// Buffer(Queue)

	public:
		// User Request Function
		// Get EndPoint
		// Force Disconnect
		// Send Packet Request




	private:
		friend class Acceptor;

		// Inner Function To Manangement Session
		// Buffer Management
		// IOCP Request
	};


	using LinkPtr = RefCountPtr<Link>;
}