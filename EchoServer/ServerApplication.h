#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/EngineIO.h"

namespace Network
{
	class Acceptor;
	class IOService;
	class Link;
}

class ServerApplication
{
private:
	std::shared_ptr<Network::IOService> _service;
	std::shared_ptr<Network::Acceptor> _acceptor;
public:
	ServerApplication();

	bool Open();

	static void OnAccepted(std::shared_ptr<Network::Link>);
	static void OnConnected(std::shared_ptr<Network::Link>);
	static void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::InputMemoryStream>);
	static void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

