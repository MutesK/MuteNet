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
		static void(*OnAccepted)(std::shared_ptr<Link>);
		static void(*OnConnected)(std::shared_ptr<Link>);
		static void(*OnRecived)(std::shared_ptr<Link>, std::shared_ptr<Util::MemoryStream>);
		static void(*OnSended)(std::shared_ptr<Link>, size_t SendedSize);
	};
}

