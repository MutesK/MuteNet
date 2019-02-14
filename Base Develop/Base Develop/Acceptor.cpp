#include "Acceptor.h"



Acceptor::Acceptor(std::shared_ptr<TcpSocket>& _listen_socket,
	SocketAddress& address,
	std::function<void(std::shared_ptr<TcpSocket>)>& onAccept)
	: _listensocket(_listen_socket),
	_address(address), _callback(std::move(onAccept))
{
	SetThreadName(std::string("Acceptor Thread"));
}


Acceptor::~Acceptor()
{
}

void Acceptor::DoWork()
{
	while (!_isthreadwork)
	{
		std::shared_ptr<TcpSocket> ret = _listensocket->Accept(_address);

		if (ret->get_socket() == INVALID_SOCKET)
			continue;

		_callback(ret);
	}
}

void Acceptor::EmitWakeupSignal()
{
	_event.SetEvent();
}
