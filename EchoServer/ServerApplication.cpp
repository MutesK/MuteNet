
#include "ServerApplication.h"
#include "../MuteNet/Link.h"

using namespace Network;

ServerApplication::ServerApplication()
{
	_acceptor = std::make_unique<Network::Acceptor>();
	_service = std::make_unique<Network::IOService>();
	_timepoint = std::chrono::high_resolution_clock::now();

	EngineIO::OnAccepted = std::bind(&ServerApplication::OnAccepted, this, std::placeholders::_1);
	EngineIO::OnRecived = std::bind(&ServerApplication::OnRecived, this, std::placeholders::_1, std::placeholders::_2);
	EngineIO::OnSended = std::bind(&ServerApplication::OnSended, this, std::placeholders::_1, std::placeholders::_2);
}

bool ServerApplication::Open()
{
	if (!_service->Initialize(8, INFINITE))
		return false;

	if (!_acceptor->Initialize(_service.get(),
		"127.0.0.1", 25000))
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
	char buf[]{ "hello" };
	auto Packet = std::make_shared<Util::OutputMemoryStream>();
	Packet->Serialize(buf, 6);

	link->SendPacket(Packet);

}

void ServerApplication::OnRecived(std::shared_ptr<Link> link, std::shared_ptr<Util::InputMemoryStream> recvPacket)
{
	char buf[1000]{ '\0' };

	recvPacket->Serialize(buf, 6);
	recvBytes += recvPacket->GetRemainingDataSize();

	auto Packet = std::make_shared<Util::OutputMemoryStream>();
	Packet->Serialize(buf, 6);

	link->SendPacket(Packet);
}

void ServerApplication::OnSended(std::shared_ptr<Link> link, size_t SendedSize)
{
	sendBytes += SendedSize;
}

void ServerApplication::Monitoring()
{
	std::cout << "Send Bytes : " << sendBytes << std::endl;
	std::cout << "Recv Bytes : " << recvBytes << std::endl;
	std::cout << "Running Time : " << std::chrono::duration<double>(std::chrono::steady_clock::now() - _timepoint).count() << std::endl;

	sendBytes = recvBytes = 0;
}
