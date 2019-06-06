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
	Destroy();
	WSACleanup();
}

void Network::Init(const std::string& dnsAddress, size_t maxSession)
{
	_acceptor = std::make_unique<Acceptor>(_socketio, dnsAddress);
}

bool Network::Start()
{
	_acceptor->Start();
}


void Network::Stop()
{
	_acceptor->Stop();
}

void Network::Destroy()
{
	_acceptor->Destory();
	_socketio->ForceDestroy();
}

void Network::OnAccept(LinkHandle link)
{
	++_currentSession;
}

void Network::OnReceived(LinkHandle link, void* pContext)
{
}

void Network::OnClosed(LinkHandle link)
{
	--_currentSession;
}

void Network::OnSended(LinkHandle link, int SendSize)
{
	++_SendPacketCnt;
}
