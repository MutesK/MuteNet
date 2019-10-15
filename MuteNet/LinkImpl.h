#pragma once

#include "Socket.h"
#include "TcpSocket.h"
#include "Network.h"
#include "Link.h"

namespace MuteNet
{
	class IOContext;

	class LinkImpl;
	typedef std::shared_ptr<LinkImpl> LinkImplPtr;

	class ServerHandleImpl;
	typedef std::shared_ptr<ServerHandleImpl> ServerHandleImplPtr;

	class LinkImpl : public Link
	{
		typedef Link super;

		Network::ConnectCallbacksPtr _ConnectCallbacks;
	public:
		LinkImpl(const CallbacksPtr LinkCallback);
		LinkImpl(intptr_t socket, const CallbacksPtr LinkCallback,
			const ServerHandleImplPtr ServerHandlePtr, const sockaddr* Addr,
			size_t socketLen);
		~LinkImpl() override;

		static LinkImplPtr Connect(std::string& Host, uint16_t Port,
			Link::CallbacksPtr LinkCallbacks, Network::ConnectCallbacksPtr ConnectCallbacks);

		bool Send(const void* Data, size_t Length) override;

		std::string GetLocalIP() const override;
		uint16_t GetLocalPort() const override;
		std::string GetRemoteIP() const override;
		uint16_t GetRemotePort() const override;

		CallbacksPtr GetCallbacks();

		void Shutdown() override;
		void Close() override;
	private:
		friend class Network;
	};

}
