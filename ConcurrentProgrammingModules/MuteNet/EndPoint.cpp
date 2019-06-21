#include "MuteNetFoundation.h"
#include "EndPoint.h"

namespace Network
{
	EndPoint::EndPoint(const std::string& address, uint16_t inPort)
	{
		CreateSockAddr(address, inPort);
	}

	EndPoint::EndPoint(const EndPoint& address)
	{
		memcpy(&_sockAddr, &address._sockAddr, sizeof(sockaddr_in));
	}

	EndPoint::EndPoint(const sockaddr& sockAddr)
	{
		memcpy(&_sockAddr, &sockAddr, sizeof(sockaddr_in));
	}

	EndPoint::EndPoint(const sockaddr_in& sockAddr_in)
	{
		memcpy(&_sockAddr, &sockAddr_in, sizeof(sockaddr_in));
	}

	EndPoint::EndPoint(EndPoint&& Point)
	{
		_sockAddr = std::move(Point._sockAddr);
	}

	std::string EndPoint::GetIpAddress() const
	{
		char buffer[20] {'\0'};
		InetNtop(AF_INET, &_sockAddr, buffer, 20);

		return buffer;
	}


	void EndPoint::CreateSockAddr(const std::string& address, uint16_t port)
	{
		char buffer[6] {0};

		addrinfo info{ 0 };
		info.ai_family = AF_INET;

		addrinfo* pinfo = nullptr;

		_itoa_s(port, buffer, 6,10);
		const auto result = getaddrinfo(address.c_str(), buffer, &info, &pinfo);
		addrinfo* addr_result = pinfo;

		if(result != 0 && pinfo != nullptr)
		{
			freeaddrinfo(addr_result);
			throw;
		}

		while(!pinfo->ai_addr && pinfo->ai_next)
		{
			addr_result = addr_result->ai_next;
		}

		if(!addr_result->ai_addr)
		{
			freeaddrinfo(addr_result);
			throw;
		}

		memcpy(&_sockAddr, addr_result->ai_addr, sizeof(sockaddr_in));
		freeaddrinfo(addr_result);
	}
}

