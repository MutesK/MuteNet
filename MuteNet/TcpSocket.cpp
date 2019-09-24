#include "pch.h"
#include "TcpSocket.h"
#include "ConnectPoint.h"

namespace Network
{
	TcpSocket::TcpSocket()
		:Socket(AF_INET)
	{
		Init(AF_INET);
	}

	TcpSocket::TcpSocket(SOCKET socket)
		: Socket(socket)
	{
	}


	bool TcpSocket::Init(ADDRESS_FAMILY f)
	{
		_handle = socket(_address_family, SOCK_STREAM, IPPROTO_TCP);

		if (_handle == INVALID_SOCKET)
			return false;

		return true;
	}

	int TcpSocket::Bind(ConnectPoint& Point) const
	{
		return bind(_handle, Point.GetSocketConnectPointPtr(), Point.GetSize());
	}

	int TcpSocket::Connect(ConnectPoint& ServerPoint)
	{
		return connect(_handle, ServerPoint.GetSocketConnectPointPtr(), ServerPoint.GetSize());
	}

	std::shared_ptr<TcpSocket> TcpSocket::Accept(ConnectPoint& OUT ClientPoint)
	{
		auto size = ClientPoint.GetSize();
		auto socket = accept(_handle, ClientPoint.GetSocketConnectPointPtr(), &size);

		if (socket == SOCKET_ERROR)
			return nullptr;

		return std::make_shared<TcpSocket>(socket);
	}

	int TcpSocket::SetNagle(bool bOption)
	{
		int opt = bOption;

		return setsockopt(_handle, IPPROTO_TCP, TCP_NODELAY,
			reinterpret_cast<const char*>(& opt), sizeof(int));
	}

	bool TcpSocket::Listen(int backlog)
	{
		const auto result = ::listen(_handle, backlog);

		if (result == SOCKET_ERROR)
		{
			_lastError = WSAGetLastError();
			_handle = INVALID_SOCKET;
			return false;
		}

		return true;
	}

	int TcpSocket::SetConditionAccept(bool trigger)
	{
		return setsockopt(_handle, SOL_SOCKET, SO_CONDITIONAL_ACCEPT,
			reinterpret_cast<char*>(&trigger), sizeof(bool));
	}

	int TcpSocket::SetNoDelay(bool toggle)
	{
		return setsockopt(_handle, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(& toggle), sizeof(bool));
	}

	int TcpSocket::SetUpdateAcceptContext(SOCKET listen)
	{
		return setsockopt(_handle, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			reinterpret_cast<char*>(listen), sizeof(SOCKET));
	}

	int TcpSocket::SetUpdateConnectContext()
	{
		return setsockopt(_handle, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT,
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

		const auto result = WSARecv(_handle, pBufArr, Arrlen, &RecvSize, &Flag, static_cast<LPOVERLAPPED>(OverlappedIO), nullptr);

		const auto errorCode = WSAGetLastError();

		if ((result != SOCKET_ERROR) || (result == SOCKET_ERROR && errorCode == WSA_IO_PENDING))
			return result;

		_lastError = errorCode;
		return -1;
	}

	int TcpSocket::OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO)
	{
		DWORD SendSize = 0;
		const DWORD Flag = 0;

		const auto result = WSASend(_handle, pBufArr, Arrlen, &SendSize, Flag, static_cast<LPOVERLAPPED>(OverlappedIO), nullptr);

		const auto errorCode = WSAGetLastError();

		if ((result != SOCKET_ERROR) || (result == SOCKET_ERROR && errorCode == WSA_IO_PENDING))
			return result;

		_lastError = errorCode;
		return -1;
	}
}
