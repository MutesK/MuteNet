#pragma once

#include "../Foundation/foundation.h"

/*
	�����ּҸ� ����Ѵ�.
*/
class SocketAddress
{
public:
	SocketAddress(uint32_t address, uint16_t inport);
	SocketAddress(const SocketAddress& address);
	SocketAddress(const sockaddr& sockaddr);
	~SocketAddress();

	const sockaddr* get_socketaddress() const
	{
		return (const sockaddr *)(&_sockaddr);
	}
	
	int getSize() const
	{
		return sizeof(sockaddr);
	}

	std::string getIpAddress();
	int	getPort();
private:
	sockaddr_in _sockaddr;
};

