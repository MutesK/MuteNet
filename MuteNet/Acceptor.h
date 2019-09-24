#pragma once

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
		IOService* _service{ nullptr };
		std::shared_ptr<TcpSocket> _listen {nullptr};

		ConnectPoint _bindPoint;
		std::thread	 _acceptorThread;

		bool				_iswork;
		std::size_t			_maxSession;
	public:
		Acceptor() = default;

		bool Initialize(IOService& service,
			const std::string& ip, uint16_t port, std::size_t maxsession);

		// Acceptor Thread Start or Stop
		bool Start();
		void Stop();

		GET_SET_ATTRIBUTE(std::shared_ptr<TcpSocket>&, listen);
	private:
		void PostAccept();
		void AcceptComplete();
	};
}
