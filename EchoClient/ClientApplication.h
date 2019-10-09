#pragma once

#include "../MuteNet/framework.h"
#include "../MuteNet/Connector.h"
#include "../MuteNet/IOService.h"
#include <chrono>
#include "../MuteNet/EngineIO.h"

class ClientApplication
{
private:
	std::unique_ptr<Network::Connector> _Connector;
	std::unique_ptr<Network::IOService> _Service;

	std::chrono::steady_clock::time_point _timepoint;
	std::atomic_int64_t recvBytes = 0;
	std::atomic_int64_t sendBytes = 0;
public:
	ClientApplication();

	bool Open();

	void Monitoring();
private:
	void OnConnected(const Network::LinkPtr&);
	void OnRecived(const Network::LinkPtr&, std::shared_ptr<Util::InputMemoryStream>&);
	void OnSended(const Network::LinkPtr&, size_t SendedSize);
};

