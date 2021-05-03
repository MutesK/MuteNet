//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_NETWORKHELPERS_HPP
#define MUTENET_NETWORKHELPERS_HPP

#include "Common.h"
#include "TypeDefine.hpp"
#include "TCPLink.hpp"
#include <vector>

struct sockaddr;
struct sockaddr_in;
struct sockaddr_in6;

namespace EventLoop
{
	class IDescriptor;
}

namespace MuteNet
{
	typedef std::shared_ptr<TCPLink> LinkPtr;

	class TCPLinkImpl;
	typedef std::shared_ptr<TCPLinkImpl> TCPLinkImplPtr;
	typedef std::vector<TCPLinkImplPtr> TCPLinkImplPtrs;

	class ServerHandle;
	typedef std::shared_ptr<ServerHandle> ServerHandlePtr;

	class NetworkHelpers
	{
	public:
		class ConnectCallbacks
		{
		public:
			virtual ~ConnectCallbacks() = default;

			virtual void OnConnected(TCPLink& link) = 0;
			virtual void OnError(int ErrorCode, std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ConnectCallbacks> ConnectCallbacksPtr;

		class ListenCallbacks
		{
		public:
			virtual ~ListenCallbacks() = default;

			virtual TCPLink::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
				uint16_t RemotePort) = 0;

			virtual void OnAccepted(TCPLink& link) = 0;

			virtual void OnError(int ErrorCode, std::string ErrorMsg) = 0;
		};
		typedef std::shared_ptr<ListenCallbacks> ListenCallbacksPtr;

		class ResolveDomainNameCallbacks
		{
		public:
			virtual ~ResolveDomainNameCallbacks() = default;

			virtual void OnNameResolved(const std::string& Name, const std::string& IP) = 0;

			virtual bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) { return true; }

			virtual bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) { return true; }

			virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;

			virtual void OnFinished(void) = 0;
		};
		typedef std::shared_ptr<ResolveDomainNameCallbacks> ResolveDomainNameCallbacksPtr;

		static bool HostnameToIP(const std::string& hostName,
			ResolveDomainNameCallbacksPtr callbacks);

		static bool IPToHostName(const std::string& Ip,
			ResolveDomainNameCallbacksPtr callbacks);
	};

	class SocketDescriptorHelper
	{
	public:
		static int32_t InetPton(int af, const void* source, char* dest);

		static void InetNtop(int af, const void* source, char* dest, int32_t length);

		static int SetSocketNonblock(descriptor_t fd);

		static int SetListenSocketReuseable(descriptor_t listen);

		static int Connect(EventLoop::IDescriptor* DescriptorPtr,
			sockaddr* Ip,
			socklen_t size);

		static std::string ErrorString(int64_t ErrorCode);

		static int64_t GetLastError();

		static int32_t ParseSockaddrPort(const char* ip, struct sockaddr* addr, int* outlen);

		static int32_t InetPtonScope(int af, const char* soruce, void* dest, uint32_t* index);

		static char* mm_strdup(const char* s);

	};

	class HostnameLookup
	{
	public:
		static void Lookup(const std::string& Hostname, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks);

	protected:
		HostnameLookup(const std::string& Hostname, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks);

		NetworkHelpers::ResolveDomainNameCallbacksPtr _Callbacks;

		std::string _Hostname;

		void Callback(int a_ErrCode, struct addrinfo* a_Addr);
	};
	using HostnameLookupPtr = std::shared_ptr<HostnameLookup>;
	using HostnameLookupPtrs = std::vector<HostnameLookupPtr>;

	class IPLookup
	{
	public:
		static void Lookup(const std::string& IP, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks);

	protected:
		NetworkHelpers::ResolveDomainNameCallbacksPtr _Callbacks;

		std::string _IP;

		IPLookup(const std::string& IP, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks);

		void Callback(int Result, const char* Address);
	};
	using IPLookupPtr = std::shared_ptr<IPLookup>;
	using IPLookupPtrs = std::vector<IPLookupPtr>;
}

#endif //MUTENET_NETWORKHELPERS_HPP
