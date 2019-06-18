#include "IOEngine.h"
#include "../Exception/Exception.h"

namespace Network
{
	IOEngine::IOEngine()
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
			throw "WSA Startup Error " + WSAGetLastError();
	}

	IOEngine::~IOEngine()
	{
		WSACleanup();
	}

	void IOEngine::Start(const std::string ip, uint16_t port)
	{
		_Acceptor.Listen(ip, port, 5);

	}
}
