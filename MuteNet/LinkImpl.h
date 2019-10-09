#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "IOContext.h"
#include "Network.h"
#include "Link.h"

namespace MuteNet
{
	class IOContext;

	class LinkImpl;
	typedef std::shared_ptr<LinkImpl> LinkImplPtr;

	class LinkImpl : public Link
	{
		typedef Link super;
	public:
		LinkImpl(const CallbacksPtr LinkCallback);
		~LinkImpl() override;

		static LinkImplPtr Connect(std::string& Host, uint16_t Port,
			Link::CallbacksPtr LinkCallbacks, Network::ConnectCallbacksPtr ConnectCallbacks);

		bool Send(const void* Data, size_t Length) override;
		std::string GetLocalIP() const override;
		uint16_t GetLocalPort() const override;
		std::string GetRemoteIP() const override;
		uint16_t GetRemotePort() const override;
		void Shutdown() override;
		void Close() override;
	};

}
