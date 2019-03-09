#pragma once
/*
	네트워크파트 부분에서, 유저가 가져야될 정보를 나타냄.

	User Content에 대한 코드는 일절 들어가서는 안된다.

	- Overlapped 이중 사용 금지
		IOCP 완료 통지시에, 해당 Overlapped 값이 달라져서, RingBuffer(환형버퍼)에 
		Index 처리가 안될수 있다.
	- 이중 완료통지 금지
*/

#include "framework.h"
#include "Ringbuffer.h"
#include "Streambuffer.h"


class Session
{
public:
	Session(TcpSocketPtr& socket);
	~Session();


	bool IOCallback(DWORD cbTransferred, LPOVERLAPPED Overlapped);

	void SendPacket(std::shared_ptr<StreamBuffer> buffer);

	inline LinkHandle getHandle()
	{
		return reinterpret_cast<LinkHandle>(_socket->get_socket());
	}

private:
	bool RecvCompletionIO(DWORD Transferred);
	bool SendCompletionIO(DWORD Transferred);

	bool RecvPost();
	bool SendPost();
private:
	TcpSocketPtr			_socket;

	std::shared_ptr<RingBuffer> _SendQ;
	std::shared_ptr<RingBuffer> _RecvQ;

	OVERLAPPED				_sendOverlapped;
	OVERLAPPED				_recvOverlapped;

	std::mutex				_sendMutex;
	std::mutex				_recvMutex;

	std::atomic<bool>			_sendFlag;
	std::atomic<bool>			_recvFlag;


	std::function<void(DWORD, LPOVERLAPPED)> _OverlappedCallback;


	friend class IOCPManager;
};
