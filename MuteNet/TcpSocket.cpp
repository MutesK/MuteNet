#include "pch.h"
#include "TcpSocket.h"
#include "SocketFunctionInvoker.h"


namespace MuteNet
{
	TcpSocket::TcpSocket(ADDRESS_FAMILY f)
		:Socket(f)
	{
		Init(f);
	}


	bool TcpSocket::Init(ADDRESS_FAMILY f)
	{
		_handle = ::socket(_address_family, SOCK_STREAM, IPPROTO_TCP);

		if (_handle == INVALID_SOCKET)
		{
			throw;
		}

		return true;
	}

	int TcpSocket::SetNagle(bool bOption) const
	{
		int opt = bOption;

		return SocketDelegateInvoker::Invoke(::setsockopt, _handle, IPPROTO_TCP, TCP_NODELAY,
			reinterpret_cast<const char*>(&opt), sizeof(int));
	}

	int TcpSocket::SetNonblock()
	{
		unsigned int nonblocking = 1;

		return SocketDelegateInvoker::Invoke(ioctlsocket, FIONBIO, &nonblocking);
	}

	bool TcpSocket::Listen(int backlog)
	{
		return SocketDelegateInvoker::Invoke(listen,_handle, backlog);
	}

	int TcpSocket::SetConditionAccept(bool trigger) const
	{
		return SocketDelegateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
			reinterpret_cast<char*>(&trigger), sizeof(bool));
	}

	int TcpSocket::SetNoDelay(bool toggle) const
	{
		return SocketDelegateInvoker::Invoke(setsockopt, _handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	int TcpSocket::SetUpdateAcceptContext(SOCKET listen) const
	{
		return SocketDelegateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(const char *)&listen, sizeof(SOCKET));
	}

	int TcpSocket::SetUpdateConnectContext() const
	{
		return SocketDelegateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT,
			nullptr, 0);
	}

	int TcpSocket::Send(const void* inData, int inLen)
	{
		const auto sentbyte = send(_handle, static_cast<const char*>(inData), inLen, 0);

		if (sentbyte >= 0)
			return sentbyte;

		_lastError = WSAGetLastError();
		return 0;
	}

	int TcpSocket::Recv(void* inData, int inLen)
	{
		const auto recvbytes = recv(_handle, static_cast<char*>(inData),
			inLen, 0);

		if (recvbytes >= 0)
			return recvbytes;

		_lastError = WSAGetLastError();
		return 0;
	}

	int TcpSocket::OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO)
	{
		DWORD RecvSize = 0;
		DWORD Flag = 0;

		return SocketDelgateInvoker::Invoke(::WSARecv, _handle, pBufArr, Arrlen, &RecvSize, &Flag, (LPOVERLAPPED)OverlappedIO, nullptr);
	}

	int TcpSocket::OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO)
	{
		DWORD SendSize = 0;
		const DWORD Flag = 0;

		return SocketDelgateInvoker::Invoke(::WSASend,_handle, pBufArr, Arrlen, &SendSize, Flag, (LPOVERLAPPED)OverlappedIO, nullptr);
	}
}
