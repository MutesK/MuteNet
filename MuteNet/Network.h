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

			virtual void OnAccepted(Link& link) = 0;

			virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ListenCallbacks> ListenCallbacksPtr;

		class ResolveDomainNameCallbacks
		{
			virtual ~ResolveDomainNameCallbacks() {}

			virtual void OnNameResolved(const std::string& Name, const std::string& IP) = 0;
			
			virtual bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) { return true; }

			virtual bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) { return true; }

			virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;

			virtual void OnFinished(void) = 0;
		};
		typedef std::shared_ptr<ResolveDomainNameCallbacks> ResolveDomainNameCallbacksPtr;

		static bool Connect(const std::string& host,
			uint16_t port,
			ConnectCallbacksPtr connectCallbacks,
			Link::CallbacksPtr LinkCallbacks);

		static ServerHandlePtr Listen(uint16_t Port,
			ListenCallbacksPtr listenCallbacks);


	};
}