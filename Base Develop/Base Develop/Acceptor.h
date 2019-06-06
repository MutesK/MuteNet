#pragma once

#include "framework.h"

/*
	�ܼ��� Accept�� �ϴ� ������

	 - Select ����� �𵨿��� ȣȯ�����ʴ´�.
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

