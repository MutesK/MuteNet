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

	class UDPLinkImpl final : public ALink, public std::enable_shared_from_this<UDPLinkImpl>
	{
		std::shared_ptr<Socket> socket;
		std::shared_ptr<IODispatcher> ioDispatcher;
		bool isShutdown = false;

		std::string localIP;
		uint16_t localPort = 0;
		std::string remoteIP;
		uint16_t remotePort = 0;
	public:
		explicit UDPLinkImpl(CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher = nullptr);
		UDPLinkImpl(std::shared_ptr<Socket> socket, CallbacksPtr callbacks, std::shared_ptr<IODispatcher> dispatcher = nullptr);
		virtual ~UDPLinkImpl() override;

		void SetDispatcher(std::shared_ptr<IODispatcher> dispatcher);
		void Enable();

		// UDP 전송 (필요에 따라 목적지 주소를 지정하는 오버로딩 추가 가능)
		bool Send(const char* data, std::size_t length) override;
		bool SendTo(const char* data, std::size_t length, const std::string& ip, uint16_t port);

		std::string GetLocalIP() const override { return localIP; }
		uint16_t GetLocalPort() const override { return localPort; }
		std::string GetRemoteIP() const override { return remoteIP; }
		uint16_t GetRemotePort() const override { return remotePort; }

		ProtocolType GetProtocolType() const override { return ProtocolType::UDP; }

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