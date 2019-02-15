#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/Thread.h"
#include "SocketAddress.h"


/*
	단순히 Accept만 하는 스레드

	 - Select 입출력 모델에는 호환되지않는다.
*/

class TcpSocket;
class Acceptor : public Thread
{
public:
	Acceptor(std::shared_ptr<TcpSocket>& _listen_socket,
		SocketAddress& address,
		std::function<void(std::shared_ptr<TcpSocket>)>& onAccept);
	~Acceptor();
private:
	std::shared_ptr<TcpSocket> _listensocket;
	std::function<void(std::shared_ptr<TcpSocket>)>   _callback;
	SocketAddress			_address;
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;

};

