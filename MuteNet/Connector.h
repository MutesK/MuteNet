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
		static LPFN_CONNECTEX Connectex;
	public:
		Connector(const std::shared_ptr<IOService>& service,
			const std::string& ip, uint16_t port);

		bool Connect();
	};
}

