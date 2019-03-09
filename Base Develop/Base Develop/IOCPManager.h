#pragma once

#include "framework.h"
#include "WorkerThread.h"

class Session;
class IOCPManager final
{
public:
	IOCPManager(int workerSize);
	~IOCPManager();

	void RegisterSocket(std::shared_ptr<Session>& session);

	void Stop();
private:
	HANDLE					_IocpHandle;

	std::list<WorkerThread> _workerthreadPool;
};

