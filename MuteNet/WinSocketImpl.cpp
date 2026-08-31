#if defined(_WIN32) || defined(_WIN64)
#include "WinSocketImpl.h"
#include "WinIODispatcherImpl.h"
#include "Socket.h"

#include <mswsock.h> // AcceptEx 관련 정의가 포함된 헤더


namespace MuteNet
{
	WinSocketImpl::WinSocketImpl()
	{
		WSADATA data;
		WSAStartup(MAKEWORD(2, 2), &data);
	}

	WinSocketImpl::~WinSocketImpl()
	{
		Close();
	}

	bool WinSocketImpl::Create(int af, int type, int protocol)
	{
		_socketHandle = WSASocket(af, type, protocol, nullptr, 0, WSA_FLAG_OVERLAPPED);
		return _socketHandle != INVALID_SOCKET_HANDLE;
	}

	bool WinSocketImpl::Bind(const std::string& ip, uint16_t port)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
		return bind(_socketHandle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != SOCKET_ERROR;
	}

	bool WinSocketImpl::Listen(int backlog)
	{
		return listen(_socketHandle, backlog) != SOCKET_ERROR;
	}

	bool WinSocketImpl::Connect(const std::string& ip, uint16_t port)
	{
		sockaddr_in addr{};
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
		return connect(_socketHandle, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != SOCKET_ERROR;
	}

	void WinSocketImpl::Close()
	{
		if (IsValid())
		{
			closesocket(_socketHandle);
			_socketHandle = INVALID_SOCKET_HANDLE;
		}
	}

	bool WinSocketImpl::SetNonBlocking(bool enable)
	{
		u_long mode = enable ? 1 : 0;
		return ioctlsocket(_socketHandle, FIONBIO, &mode) == 0;
	}

	bool WinSocketImpl::SetReuseAddress(bool enable)
	{
		int opt = enable ? 1 : 0;
		return setsockopt(_socketHandle, SOL_SOCKET, SO_REUSEADDR, 
			reinterpret_cast<const char*>(&opt), sizeof(opt)) == 0;
	}

	bool WinSocketImpl::SetNoDelay(bool enable)
	{
		return false;
	}

	int WinSocketImpl::Send(const void* buffer, size_t length)
	{
		return send(_socketHandle, (const char*)buffer, length, 0);
	}

	int WinSocketImpl::Receive(void* buffer, size_t length)
	{
		return recv(_socketHandle, (char*)buffer, (int)length, 0);
	}

	SocketHandle WinSocketImpl::Accept(std::string* clientIp, uint16_t* clientPort)
	{
		sockaddr_in addr{};
		int addrLen = sizeof(addr);
		SocketHandle client = accept(_socketHandle, (sockaddr*)&addr, &addrLen);
		if (client != INVALID_SOCKET_HANDLE && clientIp) {
			char ipStr[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &addr.sin_addr, ipStr, sizeof(ipStr));
			*clientIp = ipStr;
			if (clientPort) *clientPort = ntohs(addr.sin_port);
		}
		return client;
	}

	SocketHandle WinSocketImpl::GetHandle() const
	{
		return _socketHandle;
	}

	bool WinSocketImpl::IsValid() const
	{
		return _socketHandle != INVALID_SOCKET_HANDLE;
	}

	int WinSocketImpl::SendAsync(const void* buffer, size_t length, void* overlappedContext)
	{
		auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
		ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
		ctx->Op = IOOperation::Write;
		ctx->wsabuf.buf = (CHAR *)buffer;
		ctx->wsabuf.len = (ULONG)length;

		DWORD bytesSent = 0;
		int result = WSASend(_socketHandle, &ctx->wsabuf, 1, &bytesSent, 0, &ctx->Overlapped, nullptr);
		if (result == SOCKET_ERROR)
		{
			const auto error = WSAGetLastError();
			if(error != WSA_IO_PENDING)
			{
				return false;
			}
		}

		return true;

	}
	int WinSocketImpl::ReceiveAsync(void* buffer, size_t length, void* overlappedContext)
	{
		auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
		ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
		ctx->Op = IOOperation::Read;
		ctx->wsabuf.buf = (CHAR*)buffer;
		ctx->wsabuf.len = (ULONG)length;

		DWORD bytesSent = 0;
		int result = WSARecv(_socketHandle, &ctx->wsabuf, 1, &bytesSent, 0, &ctx->Overlapped, nullptr);
		if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			return false;
		}

		return true;
	}
	bool WinSocketImpl::AcceptAsync(void* acceptor, char* outputBuffer, uint32_t receiveDataLength, void* overlappedContext)
	{
		auto* listenSock = static_cast<Socket*>(acceptor);
		if (!listenSock || !listenSock->IsValid())
			return false;

		auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
		ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
		ctx->Op = IOOperation::Accept;

		static LPFN_ACCEPTEX lpfnAcceptEx = nullptr;
		if (!lpfnAcceptEx)
		{
			DWORD bytes = 0;
			GUID guidAcceptEx = WSAID_ACCEPTEX;
			WSAIoctl(_socketHandle, SIO_GET_EXTENSION_FUNCTION_POINTER,
				&guidAcceptEx, sizeof(guidAcceptEx),
				&lpfnAcceptEx, sizeof(lpfnAcceptEx),
				&bytes, nullptr, nullptr);
		}

		if (!lpfnAcceptEx)
			return false;

		DWORD bytesReceived = 0;
		// 주소 구조체 크기 (SOCKADDR_IN + 16바이트 필수)
		DWORD dwLocalAddressLength = sizeof(sockaddr_in) + 16;
		DWORD dwRemoteAddressLength = sizeof(sockaddr_in) + 16;

		BOOL result = lpfnAcceptEx(
			listenSock->GetHandle(),
			_socketHandle,
			outputBuffer,
			receiveDataLength - (dwLocalAddressLength + dwRemoteAddressLength),
			dwLocalAddressLength,
			dwRemoteAddressLength,
			&bytesReceived,
			&ctx->Overlapped
		);

		if (!result)
		{
			int err = WSAGetLastError();
			if (err != WSA_IO_PENDING)
			{
				return false;
			}
		}

		return true;
	}
	void WinSocketImpl::Shutdown(int how)
	{
		if (IsValid())
		{
			::shutdown(_socketHandle, how);
		}
	}

	bool WinSocketImpl::ReceiveFromAsync(void* buffer, size_t length, void* overlappedContext, void* addrOut, unsigned int* addrLenOut)
	{
		auto* ctx = static_cast<OverlappedContext*>(overlappedContext);
		ZeroMemory(&ctx->Overlapped, sizeof(WSAOVERLAPPED));
		ctx->Op = IOOperation::ReadFrom; // UDP 수신 오퍼레이션 타입
		ctx->wsabuf.buf = (CHAR*)buffer;
		ctx->wsabuf.len = (ULONG)length;

		DWORD bytesReceived = 0;
		DWORD flags = 0;

		// 핵심: UDP는 WSARecvFrom을 통해 원격지 주소 포인터와 주소 길이 변수의 포인터를 함께 전달해야 함
		int result = WSARecvFrom(
			_socketHandle,
			&ctx->wsabuf,
			1,
			&bytesReceived,
			&flags,
			(sockaddr*)addrOut,
			(INT*)addrLenOut,
			&ctx->Overlapped,
			nullptr
		);

		if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
			return false;
		}

		return true;
	}

}
#endif