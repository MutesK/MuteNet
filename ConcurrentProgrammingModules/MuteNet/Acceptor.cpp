
#include "Acceptor.h"
#include "Link.h"
#include "TcpSocket.h"
#include "IOService.h"


namespace Network
{
	Acceptor::Acceptor(const std::shared_ptr<IOService>& service, const std::string& ip, uint16_t port)
		:_service(service)
	{
		_bindPoint.SetConnectPoint(ip, port);

		_listen = std::make_unique<TcpSocket>(AF_INET);
		_listen->SetReUseAddress(true);
		_listen->Bind(_bindPoint);

		_service->RegisterHandle(_listen->native_handle(), nullptr);

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		if (SOCKET_ERROR == WSAIoctl(_listen->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &AcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
			return;
	}

	void Acceptor::Start()
	{
		_listen->Listen(0);

		_acceptorThread = std::thread(std::bind(&Acceptor::PostAccept, this));
	}

	void Acceptor::PostAccept()
	{
		while(true)
		{
			// Client Pool
		}
	}
}
