#pragma once

#include "MuteNetFoundation.h"
#include "Link.h"

namespace Network
{
	/*
	 * IOService에 AcceptEx 함수를 통해 호출시키게 한다.
	 */
	class TcpSocket;
	class IOService;

	class Acceptor final : public std::enable_shared_from_this<Acceptor>
	{
	private:
		std::shared_ptr<TcpSocket> _listen {nullptr};
		std::shared_ptr<IOService> _service {nullptr};

		ConnectPoint _bindPoint;
		std::thread	 _acceptorThread;
	
	public:
		static LPFN_ACCEPTEX AcceptEx;
	public:
		Acceptor(const std::shared_ptr<IOService>& service,
			const std::string& ip, uint16_t port);

		bool Initialize();

		// Acceptor Thread Start or Stop
		bool Start();
		void Stop();

		GET_SET_ATTRIBUTE(std::shared_ptr<TcpSocket>&, listen);
	private:
		void PostAccept();
	};
}
