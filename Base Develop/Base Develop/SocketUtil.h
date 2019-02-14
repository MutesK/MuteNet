#pragma once

#include "TcpSocket.h"
#include "UdpSocket.h"
#include "SocketAddress.h"

using UdpSocketPtr = std::shared_ptr<UdpSocket>;
using TcpSocketPtr = std::shared_ptr<TcpSocket>;

class SocketUtil
{
public:
	static std::shared_ptr<UdpSocket> CreateUDPSocket(ADDRESS_FAMILY family)
	{
		return std::make_shared<UdpSocket>(family);
	}
	static std::shared_ptr<TcpSocket> CreateTCPSocket(ADDRESS_FAMILY family)
	{
		return std::make_shared<TcpSocket>(family);
	}
};

