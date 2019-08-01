#pragma once

#include "MuteNetFoundation.h"
#include "ConnectPoint.h"

namespace Network
{
	/*
	 * IOService에 AcceptEx 함수를 통해 호출시키게 한다.
	 */
	class TcpSocket;
	class IOService;

	class Acceptor final
	{
	private:
		std::unique_ptr<TcpSocket> _listen {nullptr};
		std::shared_ptr<IOService> _service {nullptr};

		ConnectPoint _bindPoint;
		std::thread	 _acceptorThread;
	public:
		static LPFN_ACCEPTEX AcceptEx;
	public:
		Acceptor(const std::shared_ptr<IOService>& service,
			const std::string& ip, uint16_t port);

		void Start();
		void Stop();
	private:
		void PostAccept();
		void AcceptCompletion();
		
	};
}
