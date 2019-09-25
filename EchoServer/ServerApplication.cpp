
#include "ServerApplication.h"
#include "../MuteNet/IOService.h"
#include "../MuteNet/Acceptor.h"
#include "../MuteNet/Link.h"

using namespace Network;

ServerApplication::ServerApplication()
{
	_acceptor = std::make_shared<Network::Acceptor>();
	_service = std::make_shared<Network::IOService>();

	EngineIO::OnAccepted = &OnAccepted;
	EngineIO::OnConnected = &OnConnected;
	EngineIO::OnRecived = &OnRecived;
	EngineIO::OnSended = &OnSended;
}

bool ServerApplication::Open()
{
	if (!_service->Initialize(8, INFINITE))
		return false;

	if (!_acceptor->Initialize(*_service.get(),
		"localhost", 25000))
	{
		std::cout << WSAGetLastError() << std::endl;
		return false;
	}

	if (!_acceptor->Start())
		return false;

	return true;
}

void ServerApplication::OnAccepted(std::shared_ptr<Link> link)
{
	std::cout << "Accepted\n";
}

void ServerApplication::OnConnected(std::shared_ptr<Link>)
{
}

void ServerApplication::OnRecived(std::shared_ptr<Link>, std::shared_ptr<Util::InputMemoryStream>)
{
}

void ServerApplication::OnSended(std::shared_ptr<Link>, size_t SendedSize)
{
}

