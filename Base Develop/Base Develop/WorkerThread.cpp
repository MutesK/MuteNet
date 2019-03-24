#include "WorkerThread.h"
#include "IOCPManager.h"

WorkerThread::WorkerThread(HANDLE iocp)
	:_hIOCP(iocp)
{
	for (int i = 0; 10 > i; ++i)
	{
		std::thread t(std::bind(&WorkerThread::DoWork, this));
		SetThreadName(t.native_handle(), "IOCP Worker Thread");
		workers.emplace_back(t);
	}
}

WorkerThread::~WorkerThread()
{
	Stop();
}

void WorkerThread::Stop()
{
	for (auto& thread : workers)
	{
		thread.join();
		thread.detach();
	}
}

void WorkerThread::DoWork()
{
	DWORD cbTransferred;
	LPOVERLAPPED pOverlapped;
	void* pCallback;

	while (true)
	{
		cbTransferred = 0;
		pOverlapped = nullptr;
		pCallback = nullptr;

		int retval = GetQueuedCompletionStatus(_hIOCP, &cbTransferred, (PULONG_PTR)pCallback, &pOverlapped, INFINITE);

		if (pOverlapped == nullptr && pCallback == nullptr && cbTransferred == 0)
		{
			PostQueuedCompletionStatus(_hIOCP, 0, 0, 0);
			break;
		}

		// IO Callback
		std::function<void(DWORD, LPOVERLAPPED)> *pFunction = (std::function<void(DWORD, LPOVERLAPPED)> *)pCallback;
		(*pFunction)(cbTransferred, pOverlapped);
	}
}