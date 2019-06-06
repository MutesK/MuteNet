#include "IOCPManager.h"
#include "Session.h"

IOCPManager::IOCPManager(int workerSize)
{
	_IocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);

	for (int i = 0; i < workerSize; i++)
	{
		_workerthreadPool.emplace_back(WorkerThread(_IocpHandle));
	}
}


IOCPManager::~IOCPManager()
{
	ForceDestroy();
}

void IOCPManager::RegisterSocket(std::shared_ptr<Session>& session)
{
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->getHandle()), _IocpHandle,
		reinterpret_cast<ULONG64>(&session->_IOCallback), 0);
}

void IOCPManager::ForceDestroy()
{
	PostQueuedCompletionStatus(_IocpHandle, 0, 0, 0);

	_workerthreadPool.clear();
}