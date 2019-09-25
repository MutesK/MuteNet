#include "ClientApplication.h"
#include "../MuteNet/Link.h"

ClientApplication::ClientApplication()
{
	_Service = std::make_unique<Network::IOService>();
	_Connector = std::make_unique<Network::Connector>(_Service.get());
}

bool ClientApplication::Open()
{
	if (!_Service->Initialize(8, INFINITE))
		return false;

	if (!_Connector->Connect("localhost", 25000))
		return false;
}

void ClientApplication::OnConnected(std::shared_ptr<Network::Link> link)
{
	std::cout << "Connected !!!! \n";
}

void ClientApplication::OnRecived(std::shared_ptr<Network::Link> link, std::shared_ptr<Util::InputMemoryStream> buffer)
{
	std::cout << "Recived\n";
	char buf[1000]{ '\0' };

	buffer->Serialize(buf, 6);
	std::cout << "recv data : " << buf << std::endl;

	auto packet = std::dynamic_pointer_cast<Util::OutputMemoryStream>(buffer);
	packet->Serialize(buf, 6);

	link->SendPacket(packet);
}

void ClientApplication::OnSended(std::shared_ptr<Network::Link> link, size_t SendedSize)
{
	std::cout << "Sended\n";
}
