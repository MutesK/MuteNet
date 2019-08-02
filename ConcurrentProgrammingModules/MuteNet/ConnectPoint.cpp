#include "MuteNetFoundation.h"
#include "ConnectPoint.h"

namespace Network
{
	ConnectPoint::ConnectPoint(ConnectPoint&& Point) noexcept
	{
		_sockAddr = Point._sockAddr;
	}

	std::string ConnectPoint::GetIpConnectPoint() const
	{
		char buffer[20] {'\0'};
		InetNtop(AF_INET, &_sockAddr, buffer, 20);

		return buffer;
	}

	void ConnectPoint::CreateSockAddr(const std::string& ConnectPoint, uint16_t port)
	{
		char buffer[6] {0};

		addrinfo info{ 0 };
		info.ai_family = AF_INET;

		addrinfo* pinfo = nullptr;

		_itoa_s(port, buffer, 6,10);
		const auto result = getaddrinfo(ConnectPoint.c_str(), buffer, &info, &pinfo);
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

