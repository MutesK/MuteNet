#pragma once

#include "framework.h"
#include "Link.h"

namespace Network
{
	class Link;

	class EngineIO
	{
	public:
		// Network IO Function Delegate
		static std::function<void(std::shared_ptr<Link>)> OnAccepted;
		static std::function<void(std::shared_ptr<Link>)> OnConnected;
		static std::function<void(std::shared_ptr<Link>, std::shared_ptr<Util::InputMemoryStream>)> OnRecived;
		static std::function<void(std::shared_ptr<Link>, size_t SendedSize)> OnSended;
	};
}

