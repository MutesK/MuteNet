#pragma once

#include "IOContext.h"
#include "Link.h"

namespace Network
{
	class TcpSocket;
	class IOService;

	class Connector final
	{
	private:
		ConnectPoint _serverPoint;
		IOService* _service;

	public:
		static LPFN_CONNECTEX Connectex;
	public:
		Connector(IOService* service);

		bool Connect(const std::string& ip, uint16_t port);
	};
}

