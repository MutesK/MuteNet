#pragma once

#include "../Foundation/foundation.h"
#include "WorkerThread.h"
#include "SocketUtil.h"


class IOCPManager final
{
public:
	IOCPManager(int workerSize);
	~IOCPManager();

	// 소켓 IOCP에 등록, CallbackPtr에 GQCS에 나오는 인자값 넣는다. -> Session넣어줌.
	void RegisterSocket(TcpSocketPtr& socket, void* CallbackPointer);

	void Stop();
private:
	HANDLE					_IocpHandle;
	std::list<WorkerThread> _workerthreadPool;
};

