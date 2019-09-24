#include "ClientApplication.h"
#include "../MuteNet/IOService.h"
#include "../MuteNet/Connector.h"
#include "../MuteNet/Link.h"

using namespace Network;

ClientApplication::ClientApplication()
{
	_service = std::make_shared<Network::IOService>();
	_connector = std::make_shared<Network::Connector>(_service);
	


	EngineIO::OnConnected = &OnConnected;
	EngineIO::OnRecived = &OnRecived;
	EngineIO::OnSended = &OnSended;
}

bool ClientApplication::Open()
{
	if (!_service->Initialize(4, INFINITE))
		return false;

	if (!_connector->Connect("localhost", 25000))
		return false;

	return true;
}

void ClientApplication::OnConnected(std::shared_ptr<Network::Link> link)
{
	std::cout << "Connected\n";
	std::cout << "IP : " << link->get_endPoint().GetIP() <<
		" PORT : " << link->get_endPoint().GetPort() << std::endl;
}

void ClientApplication::OnRecived(std::shared_ptr<Network::Link> link, std::shared_ptr<Util::InputMemoryStream> packet)
{
	std::cout << "Recived Packet \n";

	char text[100];
	packet->Read(text);

	std::cout << "Recived Packet : " << text << std::endl;

	auto sendpacket = std::dynamic_pointer_cast<Util::OutputMemoryStream>(packet);
	link->SendPacket(sendpacket);
}

void ClientApplication::OnSended(std::shared_ptr<Network::Link>, size_t SendedSize)
{
	std::cout << "Sended Packet , Size : " << SendedSize << std::endl;
}
