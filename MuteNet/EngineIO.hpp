#pragma once

#include "EngineIO.h"
#include "Link.h"

namespace Network
{
	std::function<void(const LinkPtr&)> EngineIO::OnAccepted;
	std::function<void(const LinkPtr&)> EngineIO::OnConnected;
	std::function<void(const LinkPtr&, std::shared_ptr<Util::InputMemoryStream>&)> EngineIO::OnRecived;
	std::function<void(const LinkPtr&, size_t SendedSize)> EngineIO::OnSended;
}