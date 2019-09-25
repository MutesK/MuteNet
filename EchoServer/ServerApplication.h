#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/EngineIO.h"

#include "../MuteNet/Acceptor.h"
#include "../MuteNet/IOService.h"

class ServerApplication
{
private:
	std::unique_ptr<Network::IOService> _service;
	std::unique_ptr<Network::Acceptor> _acceptor;
public:
	ServerApplication();

	bool Open();

	static void OnAccepted(std::shared_ptr<Network::Link>);
	static void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::MemoryStream>);
	static void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

