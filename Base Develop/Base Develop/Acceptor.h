#pragma once

#include "framework.h"

/*
	단순히 Accept만 하는 스레드

	 - Select 입출력 모델에는 호환되지않는다.
*/

class Acceptor final : public std::thread
{
public:
	Acceptor(std::shared_ptr<TcpSocket>& _listen_socket);
	~Acceptor();

	void Start();
	void Stop();

	void SetOnAccept(std::function<void(TcpSocketPtr)>& Callback);
protected:
	void DoWork();
private:
	std::shared_ptr<TcpSocket> _listensocket;

	std::function<void(TcpSocketPtr)> _OnAccept;
	bool		   _flag;

	::std::condition_variable _cv;
	::std::mutex _mutex;
};

