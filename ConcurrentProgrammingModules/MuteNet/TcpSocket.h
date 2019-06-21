#pragma once

namespace Network
{
	class EndPoint;

	class TcpSocket final : public Socket
	{
	public:
		static const DWORD AddressLength = sizeof(SOCKADDR_IN) + 16;

	public:
		TcpSocket(ADDRESS_FAMILY f);
		virtual ~TcpSocket() = default;		

		void SetNagle(bool bOption);

		//bool Connect(const std::string& address);
		//bool Connect(const EndPoint& endpoint);
		//bool Connect(const std::string& ip, uint16_t port);

		virtual bool Bind(const std::string& ip, uint16_t port) override;
		virtual bool Bind(const EndPoint& endpoint) override;
		virtual bool Bind(const std::string& connection) override;

		bool Listen(int backlog);

		[[deprecated]]
		std::shared_ptr<TcpSocket> Accept();

		SOCKET AcceptEx(LPOVERLAPPED Overlapped);
		void SetEndPoint(SOCKADDR );

		int SetConditionAccept(bool trigger) const;
		int SetLoadAcceptExFunction(GUID& guid, LPFN_ACCEPTEX& Lpfn) const;
		int SetNoDelay(bool toggle) const;

		// Not Use Socket IO Function
		int Send(const void* inData, int inLen);
		int Recv(void* inData, int inLen);

		int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
		int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);



		friend class Link;
	};

}
