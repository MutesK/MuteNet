#include "pch.h"
#include "Acceptor.h"
#include "Link.h"
#include "TcpSocket.h"
#include "IOService.h"
#include "LinkManager.h"
#include "IOContext.h"

namespace Network
{
	LPFN_ACCEPTEX Acceptor::AcceptEx = nullptr;

	bool Acceptor::Initialize(IOService& service, const std::string& ip, uint16_t port)
	{
		_service = &service;
		_bindPoint.SetConnectPoint(ip, port);

		bool flag = true;

		_listen = std::make_shared<TcpSocket>(AF_INET);
		_listen->Bind(_bindPoint);

		_service->RegisterHandle(_listen->native_handle(), 
			&_listen);

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		if (SOCKET_ERROR == WSAIoctl(_listen->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &AcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
			flag = false;

		return flag;
	}

	bool Acceptor::Start()
	{
		bool flag = true;

		flag = _listen->Listen(0);
		_acceptorThread = std::thread(std::bind(&Acceptor::PostAccept, this));

		Util::ChangeThreadName(_acceptorThread.native_handle(), "Acceptor");

		return flag;
	}

	void Acceptor::Stop()
	{
		_acceptorThread.join();
	}

	void Acceptor::PostAccept()
	{
		DWORD bytes = 0;
		DWORD flags = 0;
		BYTE AcceptBuf[256];

		while(true)
		{
			if (LinkManager::UserSize() > 5000)
				continue;

			auto link = LinkManager::make_shared();

			const auto AcceptOverlapped = AcceptContext::OverlappedPool(link);

			if(FALSE == AcceptEx(_listen->socket_handle(), link->socket_handle(), AcceptBuf, 0,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, reinterpret_cast<LPOVERLAPPED>(AcceptOverlapped)))
			{
				const auto error = WSAGetLastError();
				if(error != WSA_IO_PENDING)
				{
					AcceptContext::OverlappedPool.Free(AcceptOverlapped);
					// Logger
				}
			}
		}
	}
}
