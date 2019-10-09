#include "pch.h"
#include "Connector.h"
#include "IOContext.h"
#include "LinkManager.h"
#include "IOService.h"

namespace MuteNet
{
	LPFN_CONNECTEX Connector::Connectex = nullptr;

	Connector::Connector()
		:_service(nullptr)
	{
	}

	bool Connector::Initialize(IOService* service, const std::string& ip, uint16_t port)
	{
		_service = service;

		_serverPoint.SetConnectPoint(ip, port);
		_tempClientBindPoint.SetConnectPoint();

		return true;
	}

	bool Connector::Connect()
	{
		auto link = LinkManager::Alloc();
		auto socket = link->get_socket();

		socket.Bind(_tempClientBindPoint);
		
		_service->RegisterHandle(socket.native_handle(), nullptr);

		ZeroMemory(&_connectContext.Overlapped, sizeof(OVERLAPPED));
		_connectContext.linkPtr = link;

		GUID guidConnectEx = WSAID_CONNECTEX;
		DWORD bytes = 0;

		if (!SocketDelgateInvoker::Invoke(WSAIoctl, socket.socket_handle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx, sizeof(GUID), &Connectex, sizeof(LPFN_CONNECTEX), &bytes, nullptr, nullptr))
			return false;

		if (!Connectex(socket.socket_handle(), _serverPoint.GetSocketConnectPointPtr(),
			_serverPoint.GetSize(), nullptr, 0, nullptr, &_connectContext.Overlapped))
		{
			const auto error = WSAGetLastError();
			if (error != WSA_IO_PENDING)
			{
				// Logger
				std::cout << "ConnectEx Error : " << WSAGetLastError() << std::endl;
				return false;
			}
		}

		return true;
	}

}
