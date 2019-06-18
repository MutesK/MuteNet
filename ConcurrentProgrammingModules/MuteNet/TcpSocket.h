#pragma once

namespace Network
{
	class TcpSocket final : public Socket
	{
	public:
		TcpSocket(ADDRESS_FAMILY f);
		virtual ~TcpSocket() = default;

		void setNagle(bool bOption);

		bool connect(const std::string& ip, uint16_t port);

		virtual bool Bind(const std::string& ip, uint16_t port) override;

		bool listen(int backlog);

		std::shared_ptr<TcpSocket> Accept();

		int SetNoDelay(bool toggle);

		// Not Use Socket IO Function
		int Send(const void* inData, int inLen);
		int Recv(void* inData, int inLen);

		int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
		int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);

	};

}
