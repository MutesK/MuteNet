#pragma once

#include "SocketAddress.h"

class SocketAddressFactory
{
public:
	// Name Resolve Create Socket Address
	static std::shared_ptr<SocketAddress> CreateSocketAddress(const std::string& ipv4)
	{
		auto position = ipv4.find_last_of(':');
		std::string host, service;

		if (position != std::string::npos)
		{
			host = ipv4.substr(0, position);
			service = ipv4.substr(position + 1);
		}
		else
		{
			host = ipv4;
			service = "0";
		}

		addrinfo info;
		memset(&info, 0, sizeof(addrinfo));

		info.ai_family = AF_INET;

		addrinfo* r = nullptr;
		int result = getaddrinfo(host.c_str(), service.c_str(), &info, &r);

		if (result != 0 && r != nullptr)
		{
			freeaddrinfo(r);
			return nullptr;
		}

		while (!r->ai_addr && r->ai_next)
			r = r->ai_next;

		if (!r->ai_addr)
		{
			freeaddrinfo(r);
			return nullptr;
		}

		auto ret = std::make_shared<SocketAddress>(*r->ai_addr);
		freeaddrinfo(r);

		return ret;
	}
};

