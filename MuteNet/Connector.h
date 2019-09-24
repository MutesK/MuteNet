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
		std::shared_ptr<IOService> _service{ nullptr };
		ConnectPoint _serverPoint;
	public:
		Connector(const std::shared_ptr<IOService>& service);

		bool Connect(const std::string& ip, uint16_t port);
	};
}

