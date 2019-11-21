#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"
#include "SocketUtil.h"

using namespace Util;

namespace MuteNet
{
	AcceptorPtr Acceptor::Listen(ServiceListener* Port, ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
		SOCKADDR_IN* Ip, void* key, int Backlog)
	{
		assert(Callback != nullptr);
		assert(Port != nullptr);

		const auto FuncExt = Port->GetExtension();

		assert(FuncExt != nullptr);
		assert(FuncExt->_AcceptEx != nullptr);
		assert(FuncExt->_GetAcceptExSockaddrs != nullptr);

		AcceptorPtr Ptr{new Acceptor(const_cast<ServiceListener *>(Port), Callback, ErrorCallback, Ip, key ,Backlog)};

		Ptr->InitializeListenSocket();

		return Ptr;
	}

	Acceptor::Acceptor(ServiceListener* Port, ListenerCallback& Callback, ListenerErrorCallback& ErrorCallback,
		SOCKADDR_IN* Ip, void* key, int Backlog)
		:_port(Port), _callback(std::move(Callback)), _address(Ip), _listen(INVALID_SOCKET),
		_backlog(Backlog), _errorcallback(ErrorCallback), _key(key)
	{

	}

	void Acceptor::InitializeListenSocket()
	{
		int error;

		_listen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_listen == INVALID_SOCKET)
		{
			error = WSAGetLastError();

			_errorcallback(_listen, error, SocketUtil::ErrorString(error));
			return;
		}

		_port->RegisterHandle((void *)_listen, this);

		error = std::invoke(bind, _listen,
			reinterpret_cast<sockaddr*>(_address), sizeof(SOCKADDR_IN));
		if (error != ERROR_SUCCESS)
		{
			_errorcallback(_listen, error, SocketUtil::ErrorString(error));
			return;
		}

		SocketUtil::NonBlockSocket(_listen);

		error = std::invoke(::listen, _listen, _backlog);
		if (error != ERROR_SUCCESS)
		{
			_errorcallback(_listen, error, SocketUtil::ErrorString(error));
			return;
		}
	}

	void Acceptor::StartAccept()
	{
		const auto FuncExt = _port->GetExtension();

		auto pTask = new AcceptorTask(this, _port);
		int error = 0;
		DWORD Pending;

		if (pTask->_socket == INVALID_SOCKET)
		{
			error = WSAGetLastError();

			_errorcallback(pTask->_socket, error, SocketUtil::ErrorString(error));
			return;
		}
			
		setsockopt(pTask->_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				reinterpret_cast<char*>(&_listen), sizeof(_listen));

		SocketUtil::NonBlockSocket(pTask->_socket);

		pTask->_AcceptOverlapped._CallbackPtr = OnAccepted;
		_port->RegisterHandle(reinterpret_cast<char*>(pTask->_socket), nullptr);

		if (!FuncExt->_AcceptEx(_listen, pTask->_socket, pTask->_addrbuf, 0, pTask->_buflen / 2,
			pTask->_buflen / 2, &Pending, &pTask->_AcceptOverlapped._Overlapped))
		{
			error = WSAGetLastError();

			_errorcallback(pTask->_socket, error, SocketUtil::ErrorString(error));
			return;
		}
		
	}

	void Acceptor::OnAccepted(Overlapped* pOverlapped, uintptr_t socket, int transferredBytes, int Success)
	{
		auto acceptorTask = reinterpret_cast<AcceptorTask*>(pOverlapped);
		auto acceptor = acceptorTask->_acceptor;
		auto port = acceptor->_port;

		const auto FuncExt = port->GetExtension();

		SOCKADDR* LocalSockaddr = nullptr, * RemoteSockaddr = nullptr;
		int LocalSocketLength = 0, RemoteSocketLength = 0;

		FuncExt->_GetAcceptExSockaddrs(acceptorTask->_addrbuf, 0,
			acceptorTask->_buflen / 2, acceptorTask->_buflen / 2, &LocalSockaddr, &LocalSocketLength, &RemoteSockaddr,
			&RemoteSocketLength);

		acceptor->_callback(acceptorTask->_socket, LocalSockaddr, LocalSocketLength, acceptor->_key);
		delete acceptorTask;

		if (!acceptor->_stopTrigger)
			acceptor->StartAccept();
	}

	void Acceptor::Start()
	{
		_stopTrigger = false;

		for (int i = 0; i < 4; i++)
		{
			StartAccept();
		}
	}

	void Acceptor::Stop()
	{
		_stopTrigger = true;
	}

}
