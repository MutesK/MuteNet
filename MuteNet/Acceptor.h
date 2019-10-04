#pragma once

#include "Link.h"
#include "IOContext.h"

namespace Network
{
	/*
	 * IOService�� AcceptEx �Լ��� ���� ȣ���Ű�� �Ѵ�.
	 */
	class TcpSocket;
	class IOService;

	class Acceptor final
	{
	private:
		IOService*					_service{ nullptr };
		std::unique_ptr<TcpSocket>  _listen {nullptr};

		ConnectPoint				_bindPoint;
		AcceptContext				_acceptContext;
	public:
		static LPFN_ACCEPTEX AcceptEx;
	public:
		Acceptor() = default;
		~Acceptor() = default;

		bool Initialize(IOService* service,
			const std::string& ip, uint16_t port);

		bool Open();

		void PostAccept();
	private:
		TcpSocket* get_listen();

		void AcceptCompleteIO();

		friend class IOService;
	};

	inline TcpSocket* Acceptor::get_listen()
	{
		return _listen.get();
	}
}
