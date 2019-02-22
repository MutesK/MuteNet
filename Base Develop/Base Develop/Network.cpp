#include "Network.h"



Network::Network()
{
}


Network::~Network()
{
}

void Network::Start(NetworkMode networkMode, SocketAddress& address)
{
	initIOCPMoudle(networkMode, address);

	_acceptor->Start();
}

void Network::Stop()
{
	_acceptor->Stop();
	_iocpIO->Stop();
}

void Network::initIOCPMoudle(NetworkMode& Mode, SocketAddress& address)
{
	if (Mode == Server)
	{
		_socket = SocketUtil::CreateTCPSocket(AF_INET);
		_socket->bind(address);
		_socket->listen(0);


		_iocpIO = std::make_unique<IOCPManager>(5);
	}
}

void Network::OnAccept(TcpSocketPtr ptr)
{
	auto session = _sessionManager.get_shared(ptr);
	
	_iocpIO->RegisterSocket(ptr, );
}