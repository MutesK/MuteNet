#pragma once
#include "Socket.h"

namespace Network
{
	class EndPoint;
	class TcpSocket final : public Socket
	{
	public:
		TcpSocket(ADDRESS_FAMILY f);
		virtual ~TcpSocket() = default;		

		int  Bind(ConnectPoint& Point);
		void SetNagle(bool bOption);
		bool Listen(int backlog);

		int SetConditionAccept(bool trigger) const;
		int SetNoDelay(bool toggle) const;

		// Not Use Socket IO Function
		int Send(const void* inData, int inLen);
		int Recv(void* inData, int inLen);

		int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
		int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
	};
}
