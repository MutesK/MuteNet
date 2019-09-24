#include "pch.h"
#include "Connector.h"
#include "IOContext.h"
#include "LinkManager.h"
#include "IOService.h"
#include "EngineIO.hpp"

namespace Network
{
	Connector::Connector(const std::shared_ptr<IOService>& service)
		:_service(service)
	{
	}

	bool Connector::Connect(const std::string& ip, uint16_t port)
	{
		_serverPoint.SetConnectPoint(ip, port);

		std::shared_ptr<TcpSocket> socket = std::make_shared<TcpSocket>(AF_INET);
		if (SOCKET_ERROR == socket->Connect(_serverPoint))
			return false;

		_service->RegisterHandle(socket->native_handle(), nullptr);

		auto link = LinkManager::make_shared(socket, _serverPoint);


		return true;
	}

}
