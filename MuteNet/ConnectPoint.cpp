#include "pch.h"
#include "ConnectPoint.h"

namespace Network
{
	ConnectPoint::ConnectPoint(ConnectPoint&& Point) noexcept
	{
		_sockAddr = Point._sockAddr;
	}

	std::string ConnectPoint::GetIP() const
	{
		char buffer[20] {'\0'};
		InetNtop(AF_INET, &_sockAddr, buffer, 20);

		return buffer;
	}

	void ConnectPoint::ConvertDNS(const std::string& dns, uint16_t port)
	{
		struct addrinfo stinfo;
		struct addrinfo* resultInfo;
		char portStr[10]{ 0 };

		memset(&stinfo, 0, sizeof(addrinfo));
		stinfo.ai_flags = AI_PASSIVE;
		stinfo.ai_family = AF_INET;
		stinfo.ai_socktype = SOCK_STREAM;

		_itoa_s(port, portStr, 10, 10);

		const auto err = ::getaddrinfo(dns.c_str(), portStr, &stinfo, &resultInfo);

		if(0 != err)
		{
			std::cout << "ConnectPoint Error : " << WSAGetLastError() << std::endl;
			return;
		}

		memcpy(&_sockAddr, resultInfo->ai_addr, sizeof(sockaddr_in));
		freeaddrinfo(resultInfo);
	}

	void ConnectPoint::ConvertDNS()
	{
		_sockAddr.sin_port = 0;
		_sockAddr.sin_family = AF_INET;
		_sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	}
}

