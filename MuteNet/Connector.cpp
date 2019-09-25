#include "pch.h"
#include "Connector.h"
#include "IOContext.h"
#include "LinkManager.h"
#include "IOService.h"

namespace Network
{
	LPFN_CONNECTEX Connector::Connectex = nullptr;

	Connector::Connector(const std::shared_ptr<IOService>& service, const std::string& ip, uint16_t port)
		:_service(service)
	{
		_serverPoint.SetConnectPoint(ip, port);
	}

	bool Connector::Connect()
	{
		auto link = LinkManager::make_shared();
		auto& socket = link->get_socket();
		socket.Bind(_serverPoint);

		_service->RegisterHandle(socket.native_handle(), nullptr);

		const auto ConnectOverlapped = ConnectContext::OverlappedPool(link);

		GUID guidConnectEx = WSAID_CONNECTEX;
		DWORD bytes = 0;
		if(SOCKET_ERROR == WSAIoctl(link->socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx, sizeof(GUID), &Connectex, sizeof(LPFN_CONNECTEX), &bytes,
			reinterpret_cast<LPOVERLAPPED>(ConnectOverlapped), nullptr))
		{
			if(WSAGetLastError() != WSA_IO_PENDING)
			{
				ConnectContext::OverlappedPool.Free(ConnectOverlapped);
				// Logger
				return false;
			}
		}

		return true;
	}

}
