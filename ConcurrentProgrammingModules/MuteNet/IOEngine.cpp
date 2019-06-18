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

	void IOEngine::StartServer(const std::string ip, uint16_t port)
	{
		_Acceptor.SetOnAccept(std::bind(&IOEngine::InterAccept, this, std::placeholders::_1, std::placeholders::_2));

		_Acceptor.Listen(ip, port, 5);
		_IOService.Initialize(5, INFINITE);
	}
}
