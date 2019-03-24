#include "Network.h"
#include "SessionManager.h"


Network::Network()
{
	WSADATA data;
	WSAStartup(MAKEWORD(2, 2), &data);
}


Network::~Network()
{
	Stop();
	WSACleanup();
}

void Network::Start(NetworkMode networkMode, std::string resolveDNSAddress,
	size_t maxSession)
{
	auto address = SocketAddressFactory::CreateSocketAddress(resolveDNSAddress);

	initIOCPMoudle(networkMode, address);

	_maxSession = maxSession;
}

void Network::Stop()
{
	_acceptor->Stop();
	_iocpIO->Stop();
}

void Network::initIOCPMoudle(NetworkMode& Mode, std::shared_ptr<SocketAddress>& address)
{
	if (Mode == Server)
	{
		TcpSocketPtr _socket = SocketUtil::CreateTCPSocket(AF_INET);
		_socket->bind(*address);
		_socket->listen(0);


		_iocpIO = std::make_unique<IOCPManager>(5);

		_acceptor->Start();
	}
}

void Network::OnAccept(TcpSocketPtr ptr)
{
	auto session = SessionManager::GetInstance()->AppendSession(ptr);

	_iocpIO->RegisterSocket(session->get_socket(), );

	
}