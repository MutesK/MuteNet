#include "ClientApplication.h"
#include "../MuteNet/Link.h"

using namespace Network;

ClientApplication::ClientApplication()
{
	_Service = std::make_unique<Network::IOService>();
	_Connector = std::make_unique<Network::Connector>(_Service.get());
	_timepoint = std::chrono::high_resolution_clock::now();

	EngineIO::OnConnected = std::bind(&ClientApplication::OnConnected, this, std::placeholders::_1);
	EngineIO::OnRecived = std::bind(&ClientApplication::OnRecived, this, std::placeholders::_1, std::placeholders::_2);
	EngineIO::OnSended = std::bind(&ClientApplication::OnSended, this, std::placeholders::_1, std::placeholders::_2);
}

bool ClientApplication::Open()
{
	if (!_Service->Initialize(8, INFINITE))
		return false;

	if (!_Connector->Connect("127.0.0.1", 25000))
		return false;

	return true;
}

void ClientApplication::Monitoring()
{
	std::cout << "Send Bytes : " << sendBytes << std::endl;
	std::cout << "Recv Bytes : " << recvBytes << std::endl;
	std::cout << "Running Time : " << std::chrono::duration<double>(std::chrono::steady_clock::now() - _timepoint).count() << std::endl;
	sendBytes = recvBytes = 0;
}

void ClientApplication::OnConnected(std::shared_ptr<Network::Link> link)
{
	std::cout << "Connected !!!! \n";
}

void ClientApplication::OnRecived(std::shared_ptr<Network::Link> link, std::shared_ptr<Util::InputMemoryStream> recvPacket)
{
	char buf[1000]{ '\0' };

	recvPacket->Serialize(buf, 6);
	recvBytes += recvPacket->GetRemainingDataSize();

	auto Packet = std::make_shared<Util::OutputMemoryStream>();
	Packet->Serialize(buf, 6);

	link->SendPacket(Packet);
}

void ClientApplication::OnSended(std::shared_ptr<Network::Link> link, size_t SendedSize)
{
	sendBytes += SendedSize;
}