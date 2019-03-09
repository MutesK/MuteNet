#pragma once

#include "framework.h"

/*
	�ܼ��� Accept�� �ϴ� ������

	 - Select ����� �𵨿��� ȣȯ�����ʴ´�.
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

