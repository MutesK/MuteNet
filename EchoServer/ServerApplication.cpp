
#include "ServerApplication.h"
#include "../MuteNet/Link.h"

using namespace Network;

ServerApplication::ServerApplication()
{
	_acceptor = std::make_unique<Network::Acceptor>();
	_service = std::make_unique<Network::IOService>();

	EngineIO::OnAccepted = &OnAccepted;
	EngineIO::OnRecived = &OnRecived;
	EngineIO::OnSended = &OnSended;
}

bool ServerApplication::Open()
{
	if (!_service->Initialize(8, INFINITE))
		return false;

	if (!_acceptor->Initialize(_service.get(),
		"localhost", 25000))
	{
		std::cout << WSAGetLastError() << std::endl;
		return false;
	}

	if (!_acceptor->Open())
		return false;

	return true;
}

void ServerApplication::OnAccepted(std::shared_ptr<Link> link)
{
	std::cout << "Accepted\n";

	char buf[]{ "hello" };
	auto Packet = std::make_shared<Util::OutputMemoryStream>();
	Packet->Serialize(buf, strlen(buf));

	link->SendPacket(Packet);

}

void ServerApplication::OnRecived(std::shared_ptr<Link> link, std::shared_ptr<Util::InputMemoryStream> buffer)
{
	std::cout << "Recived\n";
	char buf[1000]{ '\0' };

	buffer->Serialize(buf, 6);
	std::cout << "recv data : " << buf << std::endl;

	auto packet = std::dynamic_pointer_cast<Util::OutputMemoryStream>(buffer);
	packet->Serialize(buf, 6);

	link->SendPacket(packet);
}

void ServerApplication::OnSended(std::shared_ptr<Link> link, size_t SendedSize)
{
	std::cout << "Sended\n";
}

