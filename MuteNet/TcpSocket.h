#pragma once
#include "Socket.h"

namespace MuteNet
{
	class EndPoint;
	class TcpSocket final : public Socket
	{
	public:
		TcpSocket(ADDRESS_FAMILY f = AF_INET);
		virtual ~TcpSocket() = default;		

		bool Init(ADDRESS_FAMILY f);
		
		int SetNagle(bool bOption) const;
		int SetNonblock();
		bool Listen(int backlog);

		int SetConditionAccept(bool trigger) const;
		int SetNoDelay(bool toggle) const;
		int SetUpdateAcceptContext(SOCKET listen) const;
		int SetUpdateConnectContext() const;

		// Not Use Socket IO Function
		int Send(const void* inData, int inLen);
		int Recv(void* inData, int inLen);

		int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
		int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
	};
}
