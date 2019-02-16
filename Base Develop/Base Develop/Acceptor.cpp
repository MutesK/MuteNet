#include "Acceptor.h"
#include "TcpSocket.h"
#include "SocketUtil.h"

Acceptor::Acceptor(std::shared_ptr<TcpSocket>& _listen_socket,
	SocketAddress& address,
	std::function<void(std::shared_ptr<TcpSocket>)>&& onAccept)
	: _listensocket(_listen_socket),
	_address(address), _callback(std::move(onAccept)), Thread()
{
	
}


Acceptor::~Acceptor()
{
}

void Acceptor::DoWork()
{
	while (!_isthreadwork)
	{
		TcpSocketPtr newSocket = _listensocket->Accept();

		_callback(newSocket);

	}
}

void Acceptor::EmitWakeupSignal()
{
	_event.SetEvent();
}
