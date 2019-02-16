#include "Connector.h"


TcpSocketPtr Connector::GetClientTcpSocket(ADDRESS_FAMILY family, SocketAddress& ServerAddress)
{
	auto socket = SocketUtil::CreateTCPSocket(family);

	socket->connect(ServerAddress);
	return socket;
}

UdpSocketPtr Connector::GetClientUdpSocket(ADDRESS_FAMILY family)
{
	auto socket = SocketUtil::CreateUDPSocket(family);
	return socket;
}