#pragma once
/*
	��Ʈ��ũ��Ʈ �κп���, ������ �����ߵ� ������ ��Ÿ��.

	User Content�� ���� �ڵ�� ���� ������ �ȵȴ�.
*/

#include "Ringbuffer.h"

class Socket;
class Session
{
public:
	Session();
	~Session();


private:
	std::shared_ptr<Socket> _socket;

	RingBuffer				_recvBuffer;
	RingBuffer				_sendBuffer;
};

