#pragma once
/*
	��Ʈ��ũ��Ʈ �κп���, ������ �����ߵ� ������ ��Ÿ��.

	User Content�� ���� �ڵ�� ���� ������ �ȵȴ�.

	- Overlapped ���� ��� ����
		IOCP �Ϸ� �����ÿ�, �ش� Overlapped ���� �޶�����, RingBuffer(ȯ������)�� 
		Index ó���� �ȵɼ� �ִ�.
	- ���� �Ϸ����� ����
*/

#include "../Foundation/foundation.h"
#include "../Foundation/Singleton.hpp"
#include "../Foundation/ObjectPoolTLS.h"
#include "Ringbuffer.h"
#include "Streambuffer.h"
#include "SocketUtil.h"


class Session
{
public:
	Session(TcpSocketPtr& socket);
	~Session();

	bool RecvPost();
	bool SendPost();

	bool RecvCompletionIO(DWORD Transferred);
	bool SendCompletionIO(DWORD Transferred);

	bool IOCallback(DWORD cbTransferred, LPOVERLAPPED Overlapped);

	GET_SET_ATTRIBUTE(OVERLAPPED, sendOverlapped);
	GET_SET_ATTRIBUTE(OVERLAPPED, recvOverlapped);
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
};

using SessionManager = CObjectPoolTLS<Session>;
static SessionManager sessionManager;

