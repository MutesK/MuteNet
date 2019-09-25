#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/EngineIO.h"
#include "../MuteNet/Connector.h"
#include "../MuteNet/IOService.h"

class ClientApplication
{
private:
	std::unique_ptr<Network::Connector> _Connector;
	std::unique_ptr<Network::IOService> _Service;

public:
	ClientApplication();

	bool Open();

	static void OnConnected(std::shared_ptr<Network::Link>);
	static void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::InputMemoryStream>);
	static void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

