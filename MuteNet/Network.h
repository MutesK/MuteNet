#pragma once

#include "framework.h"
#include "Link.h"

namespace MuteNet
{
	class Link;
	typedef std::shared_ptr<Link> LinkPtr;

	class ServerHandle;
	typedef std::shared_ptr<ServerHandle> ServerHandlePtr;

	class LinkImpl;
	typedef std::shared_ptr<LinkImpl> LinkImplPtr;
	typedef std::vector<LinkImplPtr> LinkImplPtrs;

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
		public:
			virtual ~ListenCallbacks() {}

			virtual Link::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
				uint16_t RemotePort) = 0;

			virtual void OnAccepted(Link& link) = 0;

			virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ListenCallbacks> ListenCallbacksPtr;

		class ResolveDomainNameCallbacks
		{
		public:
			virtual ~ResolveDomainNameCallbacks() {}

			virtual void OnNameResolved(const std::string& Name, const std::string& IP) = 0;
			
			virtual bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) { return true; }

			virtual bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) { return true; }

			virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;

			virtual void OnFinished(void) = 0;
		};
		typedef std::shared_ptr<ResolveDomainNameCallbacks> ResolveDomainNameCallbacksPtr;

		static LinkImplPtr Connect(const std::string& host,
			uint16_t port,
			ConnectCallbacksPtr connectCallbacks,
			Link::CallbacksPtr LinkCallbacks);

		static ServerHandlePtr Listen(uint16_t Port,
			ListenCallbacksPtr listenCallbacks);

		static bool HostnameToIP(const std::string& hostName, 
			ResolveDomainNameCallbacksPtr callbacks);

		static bool IPToHostName(const std::string& Ip,
			ResolveDomainNameCallbacksPtr callbacks);
	};
}
