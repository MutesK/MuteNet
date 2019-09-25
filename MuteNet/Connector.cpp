#include "pch.h"
#include "Connector.h"
#include "IOContext.h"
#include "LinkManager.h"
#include "IOService.h"

namespace Network
{
	LPFN_CONNECTEX Connector::Connectex = nullptr;

	Connector::Connector(IOService* service)
		:_service(service)
	{
	}

	bool Connector::Connect(const std::string& ip, uint16_t port)
	{
		_serverPoint.SetConnectPoint(ip, port);

		auto link = LinkManager::make_shared();
		auto socket = link->get_socket();

		socket->Bind(_serverPoint);
		_service->RegisterHandle(socket->native_handle(), nullptr);

		const auto ConnectOverlapped = ConnectContext::OverlappedPool(link);

		GUID guidConnectEx = WSAID_CONNECTEX;
		DWORD bytes = 0;

		if (false == SocketDelgateInvoker::Invoke(WSAIoctl, link->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx, sizeof(GUID), &Connectex, sizeof(LPFN_CONNECTEX), &bytes, nullptr, nullptr))
			return false;

		if (FALSE == Connectex(socket->socket_handle(), _serverPoint.GetSocketConnectPointPtr(),
			_serverPoint.GetSize(), nullptr, 0, nullptr, &ConnectOverlapped->Overlapped))
		{
			const auto error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				ConnectContext::OverlappedPool.Free(ConnectOverlapped);
				// Logger
				std::cout << "ConnectEx Error : " << WSAGetLastError() << std::endl;
				return false;
			}
		}

		return true;
	}

}
