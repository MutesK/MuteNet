#pragma once

#include "../Foundation/foundation.h"
#include "WorkerThread.h"
#include "SocketUtil.h"


class IOCPManager final
{
public:
	IOCPManager(int workerSize);
	~IOCPManager();

	// ���� IOCP�� ���, CallbackPtr�� GQCS�� ������ ���ڰ� �ִ´�. -> Session�־���.
	void RegisterSocket(TcpSocketPtr& socket, void* CallbackPointer);

	void Stop();
private:
	HANDLE					_IocpHandle;
	std::list<WorkerThread> _workerthreadPool;
};

