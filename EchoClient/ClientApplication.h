#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/EngineIO.h"
#include "../MuteNet/Connector.h"
#include "../MuteNet/IOService.h"
#include <chrono>

class ClientApplication
{
private:
	std::unique_ptr<Network::Connector> _Connector;
	std::unique_ptr<Network::IOService> _Service;

	std::chrono::steady_clock::time_point _timepoint;
	std::size_t recvBytes = 0;
	std::size_t sendBytes = 0;
public:
	ClientApplication();

	bool Open();

	void Monitoring();
private:
	void OnConnected(std::shared_ptr<Network::Link>);
	void OnRecived(std::shared_ptr<Network::Link>, std::shared_ptr<Util::InputMemoryStream>);
	void OnSended(std::shared_ptr<Network::Link>, size_t SendedSize);
};

