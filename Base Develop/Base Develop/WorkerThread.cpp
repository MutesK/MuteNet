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
	DWORD cbTransferred = 0;
	LPOVERLAPPED pOverlapped = nullptr;
	PULONG_PTR CompletionKey = nullptr;
	void(*pCallback)(LPOVERLAPPED, int) = nullptr;

	while (true)
	{
		cbTransferred = 0;
		pOverlapped = nullptr;
		pCallback = nullptr;

		GetQueuedCompletionStatus(_hIOCP, &cbTransferred, CompletionKey, &pOverlapped, INFINITE);

		if (pOverlapped == nullptr && pCallback == nullptr && cbTransferred == 0)
		{
			PostQueuedCompletionStatus(_hIOCP, 0, 0, 0);
			break;
		}

		
		pCallback = reinterpret_cast<void(*)(LPOVERLAPPED, int)>(CompletionKey);
		(*pCallback)(pOverlapped, cbTransferred);

	}
}