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
		ConnectPoint					_serverPoint;
		ConnectPoint					_tempClientBindPoint;
		IOService*						_service;
		ConnectContext					_connectContext;
	public:
		static LPFN_CONNECTEX Connectex;
	public:
		Connector();

		bool Initialize(IOService* service,
			const std::string& ip, uint16_t port);

		bool Connect();
	};
}

