#include "MuteNetFoundation.h"
#include "SocketAddress.h"

SocketAddress::SocketAddress(uint32_t address, uint16_t inPort)
{
	_sockAddr.sin_family = AF_INET;
	_sockAddr.sin_addr.S_un.S_addr = htonl(address);
	_sockAddr.sin_port = htons(inPort);
}

SocketAddress::SocketAddress(const SocketAddress& address)
{
	memcpy(&_sockAddr, &address._sockAddr, sizeof(sockaddr_in));
}

SocketAddress::SocketAddress(const sockaddr& sockAddr)
{
	memcpy(&_sockAddr, &sockAddr, sizeof(sockaddr_in));
}


std::string SocketAddress::getIpAddress() const
{
	char buffer[20];
	InetNtop(AF_INET, &_sockAddr, buffer, 20);

	return buffer;
}


