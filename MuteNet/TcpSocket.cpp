#include "pch.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"

namespace Network
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
			// Exception
			return false;
		}

		return true;
	}

	int TcpSocket::Bind(ConnectPoint& Point) const
	{
		return SocketDelgateInvoker::Invoke(::bind, _handle, Point.GetSocketConnectPointPtr(), Point.GetSize());
	}

	int TcpSocket::SetNagle(bool bOption) const
	{
		int opt = bOption;

		return SocketDelgateInvoker::Invoke(::setsockopt, _handle, IPPROTO_TCP, TCP_NODELAY,
			reinterpret_cast<const char*>(&opt), sizeof(int));
	}

	bool TcpSocket::Listen(int backlog)
	{
		return SocketDelgateInvoker::Invoke(listen,_handle, backlog);
	}

	int TcpSocket::SetConditionAccept(bool trigger) const
	{
		return SocketDelgateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
			reinterpret_cast<char*>(&trigger), sizeof(bool));
	}

	int TcpSocket::SetNoDelay(bool toggle) const
	{
		return SocketDelgateInvoker::Invoke(setsockopt, _handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&toggle), sizeof(bool));
	}

	int TcpSocket::SetUpdateAcceptContext(SOCKET listen) const
	{
		return SocketDelgateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			(const char *)&listen, sizeof(SOCKET));
	}

	int TcpSocket::SetUpdateConnectContext() const
	{
		return SocketDelgateInvoker::Invoke(setsockopt, _handle, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT,
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
