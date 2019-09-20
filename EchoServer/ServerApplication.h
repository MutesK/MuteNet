#pragma once

#include "../MuteNet/EngineIO.h"
#include "..//MuteNet/IOService.h"
#include "../MuteNet/Acceptor.h"

using namespace Network;

class ServerApplication
{
private:
	IOService _Service;
	Acceptor _Acceptor;
public:
	ServerApplication();

	bool Open();
	
	static void OnAccepted(std::shared_ptr<Link> link);
	static void OnRecived(std::shared_ptr<Link>, std::shared_ptr<Util::InputMemoryStream>);
	static void OnSended(std::shared_ptr<Link>, size_t);

};

