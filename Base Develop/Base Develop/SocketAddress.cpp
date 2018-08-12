#include "stdafx.h"
#include "SocketAddress.h"


SocketAddress::SocketAddress(uint32_t inAddress, uint16_t inPort)
{
	GetSockAddrIn()->sin_family = AF_INET;
	GetSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
	GetSockAddrIn()->sin_port = htons(inPort);
}

SocketAddress::~SocketAddress()
{
	std::cout << "SocketAddress ÇØÁ¦µÊ !\n";
}

SocketAddress::SocketAddress(const sockaddr& IN sockaddr)
{
	memcpy(&_addr, &sockaddr, sizeof(sockaddr));
}
size_t SocketAddress::GetSize() const
{
	return sizeof(_addr);
}

sockaddr_in* SocketAddress::GetSockAddrIn()
{
	return reinterpret_cast<sockaddr_in *>(&_addr);
}
