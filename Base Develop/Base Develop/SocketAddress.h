#pragma once

/*
	sockaddr에 대해 Type Safe하게, 
	특정 소켓 API에 고착되지 않도록 래핑한다.
*/
class SocketAddress
{
public:
	SocketAddress(uint32_t inAddress, uint16_t inPort);
	SocketAddress(const sockaddr& IN sockaddr);
	size_t GetSize() const;
	~SocketAddress();

	GET_SET_ATTRIBUTE(sockaddr, addr);
	GET_CONST_ATTRIBUTE(sockaddr, addr);
private:
	sockaddr_in* GetSockAddrIn();
private:
	sockaddr  _addr;
};

