#pragma once

#include "ISocket.h"

#if defined(_WIN32) || defined(_WIN64)
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

namespace MuteNet
{
	class WinSocketImpl : public ISocket
	{
		SocketHandle _socketHandle = INVALID_SOCKET_HANDLE;
	public:
		WinSocketImpl();
		~WinSocketImpl();


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

		int SendAsync(const void* buffer, size_t length, void* overlappedContext) override;
		int ReceiveAsync(void* buffer, size_t length, void* overlappedContext) override;

		bool AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext) override;
		void Shutdown(int how) override;

		bool ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext, void* addrOut, unsigned int* addrLenOut) override;

	};
}

#endif