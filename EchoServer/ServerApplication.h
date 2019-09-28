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

	std::chrono::steady_clock::time_point _timepoint;
	std::size_t recvBytes = 0;
	std::size_t sendBytes = 0;
public:
	ServerApplication();

	bool Open();
	void Monitoring();
private:
	void OnAccepted(std::shared_ptr<Network::Link>);
	void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::InputMemoryStream>);
	void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

