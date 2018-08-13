#pragma once


class SocketAddress;
class UDPSocket;
class TCPSocket;

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6,
};

class SocketUtil
{
public:
	static std::shared_ptr<SocketAddress> CreateIPv4FromString(const std::string& IN string);
	static std::shared_ptr<UDPSocket> CreateUDPSocket(SocketAddressFamily IN Family);
	static std::shared_ptr<TCPSocket> CreateTCPSocket(SocketAddressFamily IN Family);
};


