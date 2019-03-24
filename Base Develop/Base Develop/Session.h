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

	inline LinkHandle getHandle();

	void Send(const std::shared_ptr<StreamBuffer> buffer);
	void Recv();

	void SendNext(int numOfTransffered);

	std::shared_ptr<StreamBuffer> LockReadBuffer();
	void UnlockReadBuffer();
private:
	bool RecvPost();
	bool SendPost();

	void ReadLock();
	void ReadUnLock();
private:
	TcpSocketPtr			_socket;

	std::shared_ptr<RingBuffer> _SendQ;
	std::shared_ptr<RingBuffer> _RecvQ;

	OVERLAPPED				_sendOverlapped;
	OVERLAPPED				_recvOverlapped;

	std::mutex				_recvMutex;

	std::atomic<bool>			_sendFlag;
	std::atomic<bool>			_recvFlag;

	friend class IOCPManager;
};

inline LinkHandle Session::getHandle()
{
	return reinterpret_cast<LinkHandle>(_socket->get_socket());
}