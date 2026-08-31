#pragma once

/*
*	Windows, Unix, Linux에서 전부 사용가능하게 작업하는 소켓 추상 클래스
*/

#include "Common.h"

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
typedef SOCKET SocketHandle;
const SocketHandle INVALID_SOCKET_HANDLE = INVALID_SOCKET;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
typedef int SocketHandle;
const SocketHandle INVALID_SOCKET_HANDLE = -1;
#endif
enum class IOEvent : uint32_t
{
	None = 0,
	Read = 1 << 0, // EPOLLIN / EVFLT_READ / WSARecv
	Write = 1 << 1, // EPOLLOUT / EVFLT_WRITE / WSASend
	Error = 1 << 2, // EPOLLERR / EVFLT_ERROR 
	Hangup = 1 << 3, // EPOLLHUP / EVFLT_HANGUP
};

inline IOEvent operator|(IOEvent a, IOEvent b)
{
	return static_cast<IOEvent>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}


namespace MuteNet
{
	class ISocket
	{
	public:
		virtual ~ISocket() = default;

		// 기본 Lifecycle 관리
		virtual bool Create(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0) = 0;
		virtual bool Bind(const std::string& ip, uint16_t port) = 0;
		virtual bool Listen(int backlog = SOMAXCONN) = 0;
		virtual bool Connect(const std::string& ip, uint16_t port) = 0;
		virtual void Close() = 0;

		// 소켓 상태 및 옵션 설정
		virtual bool SetNonBlocking(bool enable) = 0;
		virtual bool SetReuseAddress(bool enable) = 0;
		virtual bool SetNoDelay(bool enable) = 0;

		// IO 작업
		virtual int Send(const void* buffer, size_t length) = 0;
		virtual int Receive(void* buffer, size_t length) = 0;

		virtual int SendAsync(const void* buffer, size_t length, void* overlappedContext) = 0;
		virtual int ReceiveAsync(void* buffer, size_t length, void* overlappedContext) = 0;
		virtual bool ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext,
			void* addrOut, unsigned int* addrLenOut) = 0;

		virtual bool AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext) = 0;
		virtual void Shutdown(int how) = 0;

		// NonBlocking Accept
		virtual SocketHandle Accept(std::string* clientIp = nullptr, uint16_t* clientPort = nullptr) = 0;

		virtual SocketHandle GetHandle() const = 0;
		virtual bool IsValid() const = 0;
	};
}