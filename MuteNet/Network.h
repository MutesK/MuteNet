#pragma once

#include "framework.h"
#include "Link.h"
#include "ServerHandle.h"

namespace MuteNet
{
	typedef std::shared_ptr<Link> LinkPtr;
	typedef std::shared_ptr<ServerHandle> ServerHandlePtr;

	class Network
	{
	public:
		class ConnectCallbacks
		{
		public:
			virtual ~ConnectCallbacks() {}

			virtual void OnConnected(Link& link) = 0;
			virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ConnectCallbacks> ConnectCallbacksPtr;

		class ListenCallbacks
		{
			virtual ~ListenCallbacks() {}

			virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
				uint16_t RemotePort) = 0;

			virtual void OnAccepted(Link& link);

			virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ListenCallbacks> ListenCallbacksPtr;


		static bool Connect(const std::string& host,
			uint16_t port,
			ConnectCallbacksPtr connectCallbacks,
			Link::CallbacksPtr LinkCallbacks);

		static ServerHandlePtr Listen(uint16_t Port,
			ListenCallbacksPtr listenCallbacks);


	};
}