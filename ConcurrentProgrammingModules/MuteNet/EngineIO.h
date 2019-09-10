#pragma once

#include "MuteNetFoundation.h"
#include "../MemoryStream/InputMemoryStream.h"
namespace Network
{
	class Link;
	class EngineIO
	{
	public:
		static void(*OnAccepted)(std::shared_ptr<Link>);
		static void(*OnConnected)(std::shared_ptr<Link>);
		static void(*OnRecived)(std::shared_ptr<Link>,
			std::shared_ptr<Util::InputMemoryStream>);
		static void(*OnSended)(std::shared_ptr<Link>,
			size_t SendedSize);
	};

}