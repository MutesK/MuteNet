#pragma once

#include "MuteNetFoundation.h"

class SocketAddress final
{
public:
	SocketAddress(uint32_t address, uint16_t inPort);
	SocketAddress(const SocketAddress& address);
	SocketAddress(const sockaddr& sockAddr);
	~SocketAddress() = default;

	const sockaddr* GetSocketAddress();

	static int GetSize();

	std::string GetIpAddress() const;
	int	GetPort() const;
private:
	sockaddr_in _sockAddr{};
};


inline const sockaddr* SocketAddress::GetSocketAddress()
{
	return reinterpret_cast<const sockaddr*>(&_sockAddr);
}

inline int SocketAddress::GetPort() const
{
	return ntohs(_sockAddr.sin_port);
}

inline int SocketAddress::GetSize()
{
	return sizeof(sockaddr);
}