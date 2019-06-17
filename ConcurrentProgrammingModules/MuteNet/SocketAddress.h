#pragma once

/*
	소켓주소를 담당한다.
*/
class SocketAddress
{
public:
	SocketAddress(uint32_t address, uint16_t inPort);
	SocketAddress(const SocketAddress& address);
	SocketAddress(const sockaddr& sockAddr);
	~SocketAddress() = default;

	const sockaddr* GetSocketAddress() const;

	int GetSize() const;

	std::string getIpAddress() const;
	int	getPort() const;
private:
	sockaddr_in _sockAddr{};
};

inline const sockaddr* SocketAddress::GetSocketAddress() const
{
	return reinterpret_cast<const sockaddr*>(&_sockAddr);
}

inline int SocketAddress::getPort() const
{
	return ntohs(_sockAddr.sin_port);
}

inline int SocketAddress::GetSize() const
{
	return sizeof(sockaddr);
}