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

		addrinfo hint;
		memset(&hint, 0, sizeof(addrinfo));
		hint.ai_family = AF_INET;

		addrinfo* result = nullptr;
		int error = getaddrinfo(host.c_str(), service.c_str(), &hint, &result);
		addrinfo* initresult = result;

		if (error != 0 && result != nullptr)
		{
			freeaddrinfo(initresult);
			return nullptr;
		}

		while (!result->ai_addr && result->ai_next)
			result = result->ai_next;

		if (!result->ai_addr)
		{
			freeaddrinfo(initresult);
			return nullptr;
		}


		auto ret = std::make_shared<SocketAddress>(*result->ai_addr);
		freeaddrinfo(initresult);

		return ret;
	}
};

