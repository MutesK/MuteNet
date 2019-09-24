#pragma once
#include "Socket.h"

namespace Network
{
	class EndPoint;
	class TcpSocket final : public Socket
	{
	public:
		TcpSocket();
		TcpSocket(SOCKET socket);

		virtual ~TcpSocket() = default;		

		bool Init(ADDRESS_FAMILY f);
		int  Bind(ConnectPoint& Point) const;

		int	 Connect(ConnectPoint& ServerPoint);
		std::shared_ptr<TcpSocket> Accept(ConnectPoint& OUT ClientPoint);


		int SetNagle(bool bOption);
		bool Listen(int backlog);

		int SetConditionAccept(bool trigger);
		int SetNoDelay(bool toggle);
		int SetUpdateAcceptContext(SOCKET listen);
		int SetUpdateConnectContext();


		// Not Use Socket IO Function
		int Send(const void* inData, int inLen);
		int Recv(void* inData, int inLen);

		int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
		int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
	};
}
