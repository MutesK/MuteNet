#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"
#include "SocketUtil.h"

using namespace Util;

namespace MuteNet
{
	AcceptorPtr Acceptor::Listen(ServiceListener* Port, ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
		SOCKADDR_IN* Ip, uint16_t flag, void* key, int Backlog)
	{
		if (Port == nullptr || Callback == nullptr)
			return nullptr;

		const auto FuncExt = Port->GetExtension();
		if (FuncExt == nullptr || FuncExt->_AcceptEx == nullptr 
			|| FuncExt->_GetAcceptExSockaddrs == nullptr)
			return nullptr;


		AcceptorPtr Ptr{new Acceptor(const_cast<ServiceListener *>(Port), Callback, ErrorCallback, Ip, key ,Backlog)};

		Ptr->InitializeListenSocket();

		return Ptr;
	}

	void Acceptor::Start(int PoolSize)
	{
		if(PoolSize > 0)
		{
			StartAcceptFixedMode(PoolSize);
		}
		else
		{
			StartAcceptSocketPool();
		}
	}

	void Acceptor::Stop()
	{
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

		make_socket_nonblocking(_listen);
	}

	void Acceptor::StartAcceptFixedMode(int PoolSize)
	{

	}

	void Acceptor::StartAcceptSocketPool()
	{
		const auto FuncExt = _port->GetExtension();

		AcceptorTask* pTask = new AcceptorTask();
		int error = 0;
		DWORD Pending;

		pTask->_acceptor = this;
		pTask->_buflen = 1;
		pTask->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (pTask->_socket == INVALID_SOCKET)
		{
			error = WSAGetLastError();

			_errorcallback(pTask->_socket, error, ErrorString(error));
			return;
		}

		setsockopt(pTask->_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			reinterpret_cast<char*>(&_listen), sizeof(_listen));

		make_socket_nonblocking(pTask->_socket);

		_port->RegisterHandle(reinterpret_cast<char*>(pTask->_socket), nullptr);

		if(FuncExt->_AcceptEx(_listen, pTask->_socket, pTask->_addrbuf, 0, pTask->_buflen / 2, 
			pTask->_buflen / 2, &Pending, &pTask->_overlapped))
		{
			
		}
	}
}
