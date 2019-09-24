
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
	EngineIO::OnRecived = &OnRecived;
	EngineIO::OnSended = &OnSended;
}

bool ServerApplication::Open()
{
	if (!_service->Initialize(8, INFINITE))
		return false;

	if (!_acceptor->Initialize(*_service.get(),
		"localhost", 25000, 5000))
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
	std::cout << "IP : " << link->get_endPoint().GetIP() <<
		" PORT : " << link->get_endPoint().GetPort() << std::endl;

	auto packet = Util::OutputMemoryStream::GenerateStream();

	char text[100]{ "hello" };
	packet->Write(text);

	link->SendPacket(packet);
}

void ServerApplication::OnRecived(std::shared_ptr<Link> link, std::shared_ptr<Util::InputMemoryStream> packet)
{
	std::cout << "Recived Packet \n";

	char text[100];
	packet->Read(text);

	std::cout << "Recived Packet : " << text << std::endl;

	auto sendpacket = std::dynamic_pointer_cast<Util::OutputMemoryStream>(packet);
	link->SendPacket(sendpacket);
}

void ServerApplication::OnSended(std::shared_ptr<Link> link, size_t SendedSize)
{
	std::cout << "Sended Packet , Size : " << SendedSize << std::endl;
}

