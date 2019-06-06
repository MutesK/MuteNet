#pragma once

#include "framework.h"

/*
	단순히 Accept만 하는 스레드

	 - Select 입출력 모델에는 호환되지않는다.
*/

class IOCPManager;

class Acceptor final
{
public:
	Acceptor(const std::shared_ptr<IOCPManager>& SocketIO, const std::string& DnsAddress);
	~Acceptor();

	void Start();
	void Stop();
	void Destory();
protected:
	void DoWork();
private:
	TcpSocketPtr	 _listensocket;
	std::shared_ptr<IOCPManager> _SocketIO;
};

