#pragma once
/*
	네트워크파트 부분에서, 유저가 가져야될 정보를 나타냄.

	User Content에 대한 코드는 일절 들어가서는 안된다.
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

