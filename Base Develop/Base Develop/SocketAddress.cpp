#include "SocketAddress.h"



SocketAddress::SocketAddress(uint32_t address, uint16_t inport)
{
	_sockaddr.sin_family = AF_INET;
	_sockaddr.sin_addr.S_un.S_addr = htonl(address);
	_sockaddr.sin_port = htons(inport);
}

SocketAddress::SocketAddress(const SocketAddress& address)
{
	memcpy(&_sockaddr, &address._sockaddr, sizeof(sockaddr_in));
}

SocketAddress::SocketAddress(const sockaddr& sockaddr)
{
	memcpy(&_sockaddr, &sockaddr, sizeof(sockaddr_in));
}

SocketAddress::~SocketAddress()
{
}


std::string SocketAddress::getIpAddress()
{
	char buffer[20];
	InetNtop(AF_INET, &_sockaddr, buffer, 20);

	return buffer;
}

int	SocketAddress::getPort()
{
	return ntohs(_sockaddr.sin_port);
}