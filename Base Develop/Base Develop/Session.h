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
