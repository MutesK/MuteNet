#pragma once


class TcpSocket final : public Socket
{
public:
	TcpSocket(ADDRESS_FAMILY f);
	virtual ~TcpSocket() = default;

	void setNagle(bool bOption);

	bool connect(SocketAddress& serverAddress);

	bool Bind(const std::shared_ptr<SocketAddress>& address) override;

	bool listen(int backlog);

	std::shared_ptr<TcpSocket> Accept();

	std::shared_ptr<SocketAddress> getAddress();

	int setNoDelay(bool toggle);

	// Not Use Socket IO Moudle Function
	int Send(const void* inData, int inLen);
	int Recv(void* inData, int inLen);

	int OverlappedIORecv(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);
	int OverlappedIOSend(WSABUF* pBufArr, int Arrlen, void* OverlappedIO);

};

