#include "ServerApplication.h"
#include "..//MuteNet/EngineIO.hpp"

ServerApplication::ServerApplication()
{
	EngineIO::OnAccepted = OnAccepted;
	EngineIO::OnRecived = OnRecived;
	EngineIO::OnSended = OnSended;
}

bool ServerApplication::Open()
{
	if(!_Service.Initialize(8, INFINITE))
		return false;

	if (!_Acceptor.Initialize(_Service,
		"127.0.0.1", 8000))
		return false;


}

void ServerApplication::OnAccepted(std::shared_ptr<Link> link)
{
	std::cout << "Connected..\n";

	const auto& EndPoint = link->get_endPoint();
	std::cout << "IP : " << EndPoint.GetIP() << std::endl;
	std::cout << "Port : " << EndPoint.GetPort() << std::endl;
}

void ServerApplication::OnRecived(std::shared_ptr<Link>
	link, std::shared_ptr<Util::InputMemoryStream> buffer)
{
	std::cout << "Recived..\n";
}

void ServerApplication::OnSended(std::shared_ptr<Link> link, size_t size)
{
	std::cout << "Sended..\n";
}
