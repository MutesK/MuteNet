#pragma once

namespace MuteNet
{
	class ServerHandle
	{
		friend class Network;
	public:
		virtual ~ServerHandle() {}

		virtual void Close() = 0;

		virtual bool IsListening() const = 0;
	};
}

