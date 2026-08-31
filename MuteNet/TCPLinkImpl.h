#pragma once

#include <memory>
#include <string>
#include <atomic>
#include <cstdint>
#include "Link.h"


namespace MuteNet
{
	class Socket;
	class IODispatcher;

	class TCPLinkImpl;
	using TCPLinkImplPtr = std::shared_ptr<TCPLinkImpl>;

	class TCPLinkImpl final : public ALink, public std::enable_shared_from_this<TCPLinkImpl>
	{
		std::shared_ptr<Socket> socket;
		std::shared_ptr<IODispatcher> ioDispatcher;
		bool isShutdown = false;

		std::string localIP;
		uint16_t localPort = 0;
		std::string RemoteIP;
		uint16_t remotePort = 0;

		std::atomic<size_t> aSyncIORequestCounter = 0;
	public:
		explicit TCPLinkImpl(CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher = nullptr);
		TCPLinkImpl(std::shared_ptr<Socket> socket, CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher = nullptr);
		virtual ~TCPLinkImpl() override;

		void SetDispatcher(std::shared_ptr<IODispatcher> dispatcher);

		void Enable();
		bool Send(const char* data, std::size_t length) override;

		std::string GetLocalIP() const override { return localIP; }
		uint16_t GetLocalPort() const override { return localPort; }
		std::string GetRemoteIP() const override { return RemoteIP; }
		uint16_t GetRemotePort() const override { return remotePort; }

		ProtocolType GetProtocolType() const override { return ProtocolType::TCP; }

		CallbacksPtr GetCallbacks() const override { return callbacks; }
		std::shared_ptr<Socket> GetSocket() const { return socket; }

		void Shutdown() override;
		void Close() override;
		void RecvPost();

	private:
		friend class ASyncSendRequest;
		friend class ASyncRecvRequest;

	};
}

