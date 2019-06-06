#include "TcpSocket.h"
#include "UdpSocket.h"
#include "Session.h"
#include "Network.h"
#include "SessionManager.h"


Session::Session(TcpSocketPtr& socket)
: _socket(socket)
{
	_IOCallback = std::bind(&Session::IOCallback, this, std::placeholders::_1,
		std::placeholders::_2);
}


Session::~Session()
{
	_socket->shutdown(ShutdownBlockMode::BothBlock);
}

void Session::IOCallback(LPOVERLAPPED Overlapped, int ioSize)
{
	if(ioSize == 0)
	{
		SessionManager::GetInstance()->RemoveSession(getHandle());
		return;
	}

	if(Overlapped == &_sendOverlapped)
	{
		SendCompletionProcess(ioSize);
	}
	else if(Overlapped == &_recvOverlapped)
	{
		RecvCompletionProcess(ioSize);
	}
}

void Session::RecvCompletionProcess(int ioSize)
{ 
	Network::OnReceived(getHandle(), nullptr);
}

void Session::SendCompletionProcess(int ioSize)
{
	Network::OnSended(getHandle(), ioSize);
}

void Session::RecvPost()
{
	_socket->OverlappedIORecv(nullptr, 0, &_recvOverlapped);
}

