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
	Stop();
}

void IOCPManager::RegisterSocket(std::shared_ptr<Session>& session)
{
	session->_OverlappedCallback = std::bind(&Session::IOCallback, session.get(), std::placeholders::_1,
		std::placeholders::_2);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(session->getHandle()), _IocpHandle,
		(ULONG64)&session->_OverlappedCallback, 0);
}

void IOCPManager::Stop()
{
	PostQueuedCompletionStatus(_IocpHandle, 0, 0, 0);

	_workerthreadPool.clear();
}