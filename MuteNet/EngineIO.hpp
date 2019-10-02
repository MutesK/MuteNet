#pragma once
#include "EngineIO.h"

namespace Network
{
	std::function<void(std::shared_ptr<Link>&)> EngineIO::OnAccepted;
	std::function<void(std::shared_ptr<Link>&)> EngineIO::OnConnected;
	std::function<void(std::shared_ptr<Link>&, std::shared_ptr<Util::InputMemoryStream>&)> EngineIO::OnRecived;
	std::function<void(std::shared_ptr<Link>&, size_t)> EngineIO::OnSended;
}