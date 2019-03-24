#pragma once
/*
	��Ʈ��ũ��Ʈ �κп���, ������ �����ߵ� ������ ��Ÿ��.

	User Content�� ���� �ڵ�� ���� ������ �ȵȴ�.

	- Overlapped ���� ��� ����
		IOCP �Ϸ� �����ÿ�, �ش� Overlapped ���� �޶�����, RingBuffer(ȯ������)�� 
		Index ó���� �ȵɼ� �ִ�.
	- ���� �Ϸ����� ����
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