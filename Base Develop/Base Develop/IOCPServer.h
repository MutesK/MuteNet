#pragma once

#include <thread>
#include "ObjectPoolTLS.hpp"

class CSession;
class IOCPServer
{
public:
	IOCPServer();
	virtual ~IOCPServer();

private:
	void OnAccept(std::shared_ptr<CSession>& Session);
private:
	std::shared_ptr<Acceptor> _acceptor;

	std::vector<std::thread> _threadPool;
	HANDLE					 _iocpHandle;
};

