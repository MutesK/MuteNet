#pragma once
#include "EngineIO.h"

namespace Network
{
	void(*EngineIO::OnAccepted)(std::shared_ptr<Link>);
	void(*EngineIO::OnConnected)(std::shared_ptr<Link>);
	void(*EngineIO::OnRecived)(std::shared_ptr<Link>, std::shared_ptr<Util::InputMemoryStream>);
	void(*EngineIO::OnSended)(std::shared_ptr<Link>, size_t SendedSize);
}