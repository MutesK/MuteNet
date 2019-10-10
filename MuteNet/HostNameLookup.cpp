#include "pch.h"
#include "HostNameLookup.h"
#include "../Foundation/TaskManager.h"
#include "SocketFunctionInvoker.h"
#include "SocketUtil.h"

namespace MuteNet
{

	void HostNameLookup::Lookup(const std::string& hostName, Network::ResolveDomainNameCallbacksPtr Callbacks)
	{
		auto DoLamdba = [hostName, Callbacks]()
		{
			HostNameLookupPtr Lookup{ new HostNameLookup(hostName, std::move(Callbacks)) };

			addrinfo hints;
			ZeroMemory(&hints, sizeof(addrinfo));

			hints.ai_protocol = IPPROTO_TCP;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_family = AF_UNSPEC;
			hints.ai_flags = AI_CANONNAME;

			addrinfo* result;
			const int errorCode = getaddrinfo(Lookup->_hostName.c_str(), nullptr, &hints, &result);

			Lookup->PreProcessCallback(errorCode, result);

		};

		Enqueue_TASK(DoLamdba);
	}

	HostNameLookup::HostNameLookup(const std::string& hostName, Network::ResolveDomainNameCallbacksPtr Callbacks)
		:_Callbacks(Callbacks), _hostName(hostName)
	{
	}

	void HostNameLookup::PreProcessCallback(int ErrorCode, addrinfo* addr) const
	{
		if (ErrorCode != 0)
		{
			_Callbacks->OnError(ErrorCode, ErrorString(ErrorCode));
			return;
		}


		auto HasResolved = false;
		const auto OrigAddr = addr;
		for (; addr != nullptr; addr = addr->ai_next)
		{
			char IP[128];
			switch (addr->ai_family)
			{
			case AF_INET:  // IPv4
			{
				auto* sin = reinterpret_cast<sockaddr_in*>(addr->ai_addr);
				if (!_Callbacks->OnNameResolvedIPv4(_hostName, sin))
				{
					HasResolved = true;
					continue;
				}
				MuteNet::inet_ntop(AF_INET, &(sin->sin_addr), IP, sizeof(IP));
				break;
			}
			case AF_INET6:  // IPv6
			{
				auto* sin = reinterpret_cast<sockaddr_in6*>(addr->ai_addr);
				if (!_Callbacks->OnNameResolvedIPv6(_hostName, sin))
				{
					HasResolved = true;
					continue;
				}
				MuteNet::inet_ntop(AF_INET6, &(sin->sin6_addr), IP, sizeof(IP));
				break;
			}
			default:
			{
				continue;
			}
			}
			_Callbacks->OnNameResolved(_hostName, IP);
			HasResolved = true;
		}

		// 지원되지 않은 Address Family
		if (!HasResolved)
		{
			_Callbacks->OnError(EAI_NONAME, ErrorString(EAI_NONAME));
		}
		else
		{
			_Callbacks->OnFinished();
		}

		freeaddrinfo(OrigAddr);
	}

}