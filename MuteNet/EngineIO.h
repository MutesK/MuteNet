#pragma once

#include "framework.h"

namespace Network
{
	class Link;
	class EngineIO
	{
	public:
		// Network IO Function Delegate
		static std::function<void(const LinkPtr&)> OnAccepted;
		static std::function<void(const LinkPtr&)> OnConnected;
		static std::function<void(const LinkPtr&, std::shared_ptr<Util::InputMemoryStream>&)> OnRecived;
		static std::function<void(const LinkPtr&, size_t SendedSize)> OnSended;
	};
}

