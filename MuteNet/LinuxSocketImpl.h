#pragma once

#include "ISocket.h"

#if defined(__linux__)

namespace MuteNet
{
	class LinuxSocketImpl : public ISocket
	{
		SocketHandle m_socket = INVALID_SOCKET_HANDLE;
	public:
		bool Create(int af, int type, int protocol) override;
		bool Bind(const std::string& ip, uint16_t port) override;
		bool Listen(int backlog) override;
		bool Connect(const std::string& ip, uint16_t port) override;
		void Close() override;

		bool SetNonBlocking(bool enable) override;
		bool SetReuseAddress(bool enable) override;
		bool SetNoDelay(bool enable) override;

		int Send(const void* buffer, size_t length) override;
		int Receive(void* buffer, size_t length) override;

		SocketHandle Accept(std::string* clientIp, uint16_t* clientPort) override;
		SocketHandle GetHandle() const override;

		bool IsValid() const override;

		bool AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext) override;
		void Shutdown(int how) override;

		bool ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext,
			void* addrOut, unsigned int* addrLenOut) override;
	};
}
#endif