#include "Acceptor.h"


Acceptor::Acceptor(std::shared_ptr<TcpSocket>& _listen_socket)
	: _listensocket(_listen_socket), std::thread(std::bind(&Acceptor::DoWork, this))
{
	SetThreadName(native_handle(), "Acceptor Thread");
}

Acceptor::~Acceptor()
{
	Stop();

	join();
	detach();
}

void Acceptor::Start()
{
	_flag = true;
}

void Acceptor::Stop()
{
	_flag = false;
}

void Acceptor::SetOnAccept(std::function<void(TcpSocketPtr)>& Callback)
{
	_flag = true;
	_OnAccept = Callback;
}

void Acceptor::DoWork()
{
	while (true)
	{
		std::unique_lock<decltype(_mutex)> lock(_mutex);
		_cv.wait(lock, [this] { return _flag;  });

		TcpSocketPtr socket = _listensocket->Accept();

		if (socket->get_socket() == INVALID_SOCKET)
			continue;
	}
}