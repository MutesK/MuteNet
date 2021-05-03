//
// Created by Mute on 2020-11-18.
//


#include "Common.h"
#include "TypeDefine.hpp"
#include "NetworkHelpers.hpp"
#include "Descriptor.h"

namespace MuteNet
{
	int32_t SocketDescriptorHelper::ParseSockaddrPort(const char* ip, struct sockaddr* addr, int* outlen)
	{
		int port;
		unsigned int if_index;
		char buf[128];
		const char* cp, * addr_part, * port_part;
		int is_ipv6;

		cp = strchr(ip, ':');
		if (*ip == '[')
		{
			size_t len;
			if (!(cp = strchr(ip, ']')))
			{
				return -1;
			}
			len = (cp - (ip + 1));
			if (len > sizeof(buf) - 1)
			{
				return -1;
			}
			memcpy(buf, ip + 1, len);
			buf[len] = '\0';
			addr_part = buf;
			if (cp[1] == ':')
			{
				port_part = cp + 2;
			}
			else
			{
				port_part = NULL;
			}
			is_ipv6 = 1;
		}
		else if (cp && strchr(cp + 1, ':'))
		{
			is_ipv6 = 1;
			addr_part = ip;
			port_part = NULL;
		}
		else if (cp)
		{
			is_ipv6 = 0;
			if (cp - ip > (int)sizeof(buf) - 1)
			{
				return -1;
			}
			memcpy(buf, ip, cp - ip);
			buf[cp - ip] = '\0';
			addr_part = buf;
			port_part = cp + 1;
		}
		else
		{
			addr_part = ip;
			port_part = NULL;
			is_ipv6 = 0;
		}

		if (port_part == NULL)
		{
			port = 0;
		}
		else
		{
			port = atoi(port_part);
			if (port <= 0 || port > 65535)
			{
				return -1;
			}
		}

		if (!addr_part)
		{
			return -1;
		} /* Should be impossible. */
#ifdef AF_INET6
		if (is_ipv6)
		{
			struct sockaddr_in6 sin6;
			memset(&sin6, 0, sizeof(sin6));

			sin6.sin6_family = AF_INET6;
			sin6.sin6_port = htons(port);
			if (1 != InetPtonScope(
				AF_INET6, addr_part, &sin6.sin6_addr, &if_index))
			{
				return -1;
			}
			if ((int)sizeof(sin6) > *outlen)
				return -1;
			sin6.sin6_scope_id = if_index;
			memset(addr, 0, *outlen);
			memcpy(addr, &sin6, sizeof(sin6));
			*outlen = sizeof(sin6);
			return 0;
		}
		else
#endif
		{
			struct sockaddr_in sin;
			memset(&sin, 0, sizeof(sin));

			sin.sin_family = AF_INET;
			sin.sin_port = htons(port);
			if (1 != SocketDescriptorHelper::InetPton(AF_INET, addr_part, reinterpret_cast<char*>(&sin.sin_addr)))
				return -1;
			if ((int)sizeof(sin) > *outlen)
				return -1;
			memset(addr, 0, *outlen);
			memcpy(addr, &sin, sizeof(sin));
			*outlen = sizeof(sin);
			return 0;
		}
	}

	int32_t SocketDescriptorHelper::InetPtonScope(int af, const char* soruce, void* dest, uint32_t* index)
	{
		int r;
		unsigned if_index;
		char* check, * cp, * tmp_src;

		*index = 0; /* Reasonable default */

		/* Bail out if not IPv6 */
		if (af != AF_INET6)
		{
			return SocketDescriptorHelper::InetPton(af, soruce, static_cast<char*>(dest));
		}

		cp = const_cast<char*>(strchr(soruce, '%'));

		/* Bail out if no zone ID */
		if (cp == NULL)
		{
			return SocketDescriptorHelper::InetPton(af, soruce, static_cast<char*>(dest));
		}

		if_index = if_nametoindex(cp + 1);
		if (if_index == 0)
		{
			/* Could be numeric */
			if_index = strtoul(cp + 1, &check, 10);
			if (check[0] != '\0')
			{
				return 0;
			}
		}
		*index = if_index;
		tmp_src = SocketDescriptorHelper::mm_strdup(soruce);
		cp = strchr(tmp_src, '%');
		*cp = '\0';
		r = SocketDescriptorHelper::InetPton(af, tmp_src, static_cast<char*>(dest));
		free(tmp_src);
		return r;
	}

	char* SocketDescriptorHelper::mm_strdup(const char* s)
	{
		size_t len = strlen(s);
		char* s2 = (char*)malloc((len + 1) * sizeof(char));
		return strcpy(s2, s);
	}

	int32_t SocketDescriptorHelper::InetPton(int af, const void* source, char* dest)
	{
		return ::inet_pton(af, (char*)source, dest);
	}

	void SocketDescriptorHelper::InetNtop(int af, const void* source, char* dest, int32_t length)
	{
		::inet_ntop(af, (char*)source, (char*)dest, length);
	}

	int SocketDescriptorHelper::SetSocketNonblock(descriptor_t fd)
	{
#ifdef _WIN32
		{
			unsigned long nonblocking = 1;
			if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR)
			{
				return SOCKET_ERROR;
			}
		}
#else
		{
			int flags;
			if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
				return -1;
			}
			if (!(flags & O_NONBLOCK)) {
				if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
					return -1;
				}
			}
		}
#endif

		return 0;
	}

	int SocketDescriptorHelper::SetListenSocketReuseable(descriptor_t listen)
	{
#if defined(SO_REUSEADDR) && !defined(_WIN32)
		int one = 1;
		return setsockopt(listen, SOL_SOCKET, SO_REUSEADDR, (void*)&one,
			(socklen_t)sizeof(one));
#else
		return 0;
#endif
	}

	int SocketDescriptorHelper::Connect(EventLoop::IDescriptor* DescriptorPtr, sockaddr* Ip, socklen_t size)
	{
		return 0;
	}

	std::string SocketDescriptorHelper::ErrorString(int64_t ErrorCode)
	{
#ifdef WIN32
		char ErrorStr[GAI_STRERROR_BUFFER_SIZE + 1];

		int MsgLen = FormatMessageA(
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK,
			nullptr,
			ErrorCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			ErrorStr,
			sizeof(ErrorStr) - 1,
			nullptr
		);

		return std::string(ErrorStr);
#else
		return strerror(ErrorCode);
#endif
	}

	int64_t SocketDescriptorHelper::GetLastError()
	{
#if defined(WIN32)
		return WSAGetLastError();
#else
		return errno;
#endif
	}


	bool
		NetworkHelpers::HostnameToIP(const std::string& hostName, NetworkHelpers::ResolveDomainNameCallbacksPtr callbacks)
	{
		HostnameLookup::Lookup(hostName, callbacks);

		return true;
	}

	bool NetworkHelpers::IPToHostName(const std::string& Ip, NetworkHelpers::ResolveDomainNameCallbacksPtr callbacks)
	{
		IPLookup::Lookup(Ip, callbacks);

		return true;
	}

	HostnameLookup::HostnameLookup(const std::string& Hostname, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks)
		:
		_Callbacks(Callbacks),
		_Hostname(Hostname)
	{
		assert(Callbacks != nullptr);
	}

	void HostnameLookup::Lookup(const std::string& Hostname, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks)
	{
		HostnameLookupPtr Lookup(new HostnameLookup(Hostname, Callbacks));

		auto AsyncLookup = [=]()
		{
			addrinfo hints;
			memset(&hints, 0, sizeof(hints));
			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_UNSPEC;
			hints.ai_flags = AI_CANONNAME;

			addrinfo* pResult = nullptr;
			int result = getaddrinfo(Lookup->_Hostname.c_str(), nullptr, &hints, &pResult);

			Lookup->Callback(result, pResult);
		};

		auto async = std::async(std::launch::async, AsyncLookup);
	}

	void HostnameLookup::Callback(int ErrCode, struct addrinfo* Addr)
	{
		if (ErrCode != 0)
		{
			_Callbacks->OnError(ErrCode, SocketDescriptorHelper::ErrorString(ErrCode));
			return;
		}

		bool Resolved = false;
		addrinfo* origin = Addr;

		for (; Addr != nullptr; Addr = Addr->ai_next)
		{
			char IP[128];
			switch (Addr->ai_family)
			{
			case AF_INET:  // IPv4
			{
				sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(Addr->ai_addr);
				if (!_Callbacks->OnNameResolvedIPv4(_Hostname, sin))
				{
					// Callback indicated that the IP shouldn't be serialized to a string, just continue with the next address:
					Resolved = true;
					continue;
				}
				SocketDescriptorHelper::InetNtop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
				break;
			}
			case AF_INET6:  // IPv6
			{
				sockaddr_in6* sin = reinterpret_cast<sockaddr_in6*>(Addr->ai_addr);
				if (!_Callbacks->OnNameResolvedIPv6(_Hostname, sin))
				{
					// Callback indicated that the IP shouldn't be serialized to a string, just continue with the next address:
					Resolved = true;
					continue;
				}
				SocketDescriptorHelper::InetNtop(AF_INET, &(sin->sin6_addr), IP, sizeof(IP));
				break;
			}
			default:
			{
				// Unknown address family, handle as if this entry wasn't received
				continue;  // for (a_Addr)
			}
			}
			_Callbacks->OnNameResolved(_Hostname, IP);
			Resolved = true;
		}

		if (!Resolved)
		{
			_Callbacks->OnError(EAI_NONAME, SocketDescriptorHelper::ErrorString(EAI_NONAME));
		}
		else
		{
			_Callbacks->OnFinished();
		}
		freeaddrinfo(origin);
	}

	IPLookup::IPLookup(const std::string& IP, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks)
		:
		_Callbacks(Callbacks),
		_IP(IP)
	{
		assert(Callbacks != nullptr);
	}

	void IPLookup::Lookup(const std::string& IP, NetworkHelpers::ResolveDomainNameCallbacksPtr Callbacks)
	{
		IPLookupPtr Lookup(new IPLookup(IP, Callbacks));

		auto AsyncLookup = [=]()
		{
			sockaddr_storage sa{ 0 };
			int len = sizeof(sa);

			int result = SocketDescriptorHelper::ParseSockaddrPort(IP.c_str(), reinterpret_cast<sockaddr*>(&sa),
				&len);

			if (result != 0)
			{
				Lookup->Callback(result, nullptr);
				return;
			}

			char HostName[NI_MAXHOST];
			char ServerInfo[NI_MAXSERV];

			result = getnameinfo(reinterpret_cast<sockaddr*>(&sa),
				static_cast<socklen_t>(len),
				HostName, sizeof(HostName),
				ServerInfo, sizeof(ServerInfo), 0);

			Lookup->Callback(result, HostName);
		};

		auto async = std::async(std::launch::async, AsyncLookup);
	}


	void IPLookup::Callback(int Result, const char* Address)
	{
		if ((Result != 0) || (Address == nullptr))
		{
			// An error has occurred, notify the error callback:
			_Callbacks->OnError(Result, SocketDescriptorHelper::ErrorString(Result));
		}
		else
		{
			// Call the success handler:
			_Callbacks->OnNameResolved(Address, _IP);
			_Callbacks->OnFinished();
		}
	}
}