#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"
#include "SocketUtil.h"

namespace MuteNet
{
	AcceptorPtr Acceptor::Listen(ServiceListener* Port, listener_callback& Callback, listener_errorcallback& ErrorCallback,
		SOCKADDR_IN* Ip, int Backlog)
	{
		if (Port == nullptr)
			return nullptr;

		const auto FuncExt = Port->GetExtension();
		if (FuncExt == nullptr || FuncExt->_AcceptEx == nullptr 
			|| FuncExt->_GetAcceptExSockaddrs == nullptr)
			return nullptr;

		AcceptorPtr Ptr{new Acceptor(const_cast<ServiceListener *>(Port), Callback, ErrorCallback, Ip, Backlog)};

		Ptr->InitializeListenSocket();

		return Ptr;
	}

	Acceptor::Acceptor(ServiceListener* Port, listener_callback& Callback, listener_errorcallback& ErrorCallback,
		SOCKADDR_IN* Ip, int Backlog)
		:_port(Port), _callback(std::move(Callback)), _address(Ip), _listen(INVALID_SOCKET),
		_backlog(Backlog), _errorcallback(ErrorCallback)
	{
	}

	void Acceptor::InitializeListenSocket()
	{
		int error;

		_listen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_listen == INVALID_SOCKET)
		{
			error = WSAGetLastError();

			_errorcallback(_listen, error, ErrorString(error));
			return;
		}

		_port->RegisterHandle((void *)_listen, nullptr);

		error = SocketDelegateInvoker::Invoke(bind, _listen,
			reinterpret_cast<sockaddr*>(_address), sizeof(SOCKADDR_IN));
		if (error != ERROR_SUCCESS)
		{
			_errorcallback(_listen, error, ErrorString(error));
			return;
		}

		error = SocketDelegateInvoker::Invoke(::listen, _listen, _backlog);
		if (error != ERROR_SUCCESS)
		{
			_errorcallback(_listen, error, ErrorString(error));
			return;
		}

	}

}
