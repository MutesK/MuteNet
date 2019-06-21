#pragma once

#include "MuteNetFoundation.h"
#include "IOContext.h"

namespace Network
{
	class TcpSocket;
	class IOService;

	class Acceptor final
	{
	private:
		std::unique_ptr<TcpSocket> _Listen;
		std::shared_ptr<IOService> _Service;

		LPFN_ACCEPTEX			   _AcceptEx;
		GUID					   _GuidAcceptEx = WSAID_ACCEPTEX;

		AcceptIOContext			   _Context;
	public:
		Acceptor(const std::shared_ptr<IOService>& service,
			const std::string& ip, uint16_t port);

		void ProcessAccept(AcceptIOContext& Context);
	private:
		void AcceptPost();

	};
}
