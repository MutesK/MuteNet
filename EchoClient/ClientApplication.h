#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/EngineIO.h"

namespace Network
{
	class Connector;
	class IOService;
	class Link;
}

class ClientApplication
{
private:
	std::shared_ptr<Network::IOService> _service;
	std::shared_ptr<Network::Connector> _connector;

public:
	ClientApplication();

	bool Open();

	static void OnConnected(std::shared_ptr<Network::Link>);
	static void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::InputMemoryStream>);
	static void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

