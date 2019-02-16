#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/ObjectPoolTLS.h"
#include "SocketUtil.h"

/*
	For Bot or Real Client Connect Class
*/
class Connector
{
public:
	static TcpSocketPtr GetClientTcpSocket(ADDRESS_FAMILY, SocketAddress& ServerAddress);
	static UdpSocketPtr GetClientUdpSocket(ADDRESS_FAMILY);
};

