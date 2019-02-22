#include "IOCPManager.h"



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

void IOCPManager::RegisterSocket(TcpSocketPtr& _socket, void* CallbackPointer)
{
	CreateIoCompletionPort(reinterpret_cast<HANDLE>(_socket->get_socket()), _IocpHandle, (ULONG64)CallbackPointer,
		0);
}

void IOCPManager::Stop()
{
	PostQueuedCompletionStatus(_IocpHandle, 0, 0, 0);

	for (auto thread : _workerthreadPool)
	{
		thread.Stop();
	}

}