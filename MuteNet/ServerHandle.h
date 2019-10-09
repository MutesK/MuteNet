#pragma once

namespace MuteNet
{
	class ServerHandle
	{
		friend class Network;
	public:
		virtual ~ServerHandle() {}

		virtual void Close() = 0;

		virtual void IsListening() const = 0;
	};
}

