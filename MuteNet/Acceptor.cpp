#include "pch.h"
#include "Acceptor.h"
#include "Link.h"
#include "TcpSocket.h"
#include "IOService.h"
#include "LinkManager.h"
#include "IOContext.h"
#include "EngineIO.hpp"

namespace Network
{
	bool Acceptor::Initialize(IOService& service, const std::string& ip, uint16_t port, std::size_t maxsession)
	{
		if (_iswork)
			return false;

		_maxSession = maxsession;

		_service = &service;
		_bindPoint.SetConnectPoint(ip, port);

		bool flag = true;

		_listen = std::make_shared<TcpSocket>(AF_INET);
		_listen->Bind(_bindPoint);

		GUID guidAcceptEx = WSAID_ACCEPTEX;
		DWORD bytes = 0;

		return flag;
	}

	bool Acceptor::Start()
	{
		bool flag = true;

		flag = _listen->Listen(0);

		if (flag)
			_iswork = true;

		_acceptorThread = std::thread(std::bind(&Acceptor::PostAccept, this));

		Util::ChangeThreadName(_acceptorThread.native_handle(), "Acceptor");


		return flag;
	}

	void Acceptor::Stop()
	{
		_iswork = false;

		_acceptorThread.join();
	}

	void Acceptor::PostAccept()
	{
		DWORD bytes = 0;
		DWORD flags = 0;
		BYTE AcceptBuf[256];

		while(_iswork)
		{
			if (LinkManager::UserSize() >= _maxSession)
				continue;

			ConnectPoint Point;
			auto socket = _listen->Accept(Point);

			if (socket == nullptr)
				continue;

			const auto LinkPtr = LinkManager::make_shared(socket, Point);
			socket->SetNagle(true);

			_service->RegisterHandle(socket->native_handle(), nullptr);

			EngineIO::OnAccepted(LinkPtr);
		}
	}

}
