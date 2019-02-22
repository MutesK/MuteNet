#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/Thread.h"
#include "SocketAddress.h"


/*
	�ܼ��� Accept�� �ϴ� ������

	 - Select ����� �𵨿��� ȣȯ�����ʴ´�.
*/

class TcpSocket;
class Acceptor : public Thread
{
public:
	Acceptor(std::shared_ptr<TcpSocket>& _listen_socket,
		std::function<void(std::shared_ptr<TcpSocket>)>&& onAccept);
	~Acceptor();
private:
	std::shared_ptr<TcpSocket> _listensocket;
	std::function<void(std::shared_ptr<TcpSocket>)>   _callback;
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;

};

