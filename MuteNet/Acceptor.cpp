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

	bool Acceptor::Initialize(IOService* service, const std::string& ip, uint16_t port)
	{
		_service = service;
		_bindPoint.SetConnectPoint(ip, port);

		bool flag = true;

		_listen = std::make_unique<TcpSocket>(AF_INET);
		_listen->Bind(_bindPoint);

		_service->RegisterHandle(_listen->native_handle(), this);

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		return SocketDelgateInvoker::Invoke(WSAIoctl, _listen->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidAcceptEx, sizeof(GUID), &AcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, nullptr, nullptr);
	}

	bool Acceptor::Open()
	{
		if (!_listen->Listen(0))
			return false;

		PostAccept();

		return true;
	}

	void Acceptor::PostAccept()
	{
		DWORD bytes = 0;
		DWORD flags = 0;
		BYTE AcceptBuf[256];

		auto link = LinkManager::make_shared();

		const auto AcceptOverlapped = new AcceptContext(link);

		if (FALSE == AcceptEx(_listen->socket_handle(), link->socket_handle(), AcceptBuf, 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, reinterpret_cast<LPOVERLAPPED>(&AcceptOverlapped->Overlapped)))
		{
			const auto error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				// Logger
				
				std::cout << "AcceptEx Error : " << WSAGetLastError() << std::endl;
			}
		}
	}
}
