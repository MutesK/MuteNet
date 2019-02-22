#include "WorkerThread.h"



WorkerThread::WorkerThread(HANDLE iocp)
	:_hIOCP(iocp)
{
	std::string threadid = "WorkerThread";
	SetThreadName(threadid);

	Start();
	Stop();
	_isthreadwork = true;
}


WorkerThread::~WorkerThread()
{
	Stop();
}

void WorkerThread::DoWork()
{
	DWORD cbTransferred;
	LPOVERLAPPED pOverlapped;


	while (!_isthreadwork)
	{
		cbTransferred = 0;
		pOverlapped = nullptr;


		int retval = GetQueuedCompletionStatus(_hIOCP, &cbTransferred, (PULONG_PTR)voidPtr, &pOverlapped, INFINITE);

		if (pOverlapped == nullptr && voidPtr == nullptr && cbTransferred == 0)
		{
			PostQueuedCompletionStatus(_hIOCP, 0, 0, 0);
			break;
		}

		(*voidPtr)(cbTransferred, pOverlapped);
	}
}

void WorkerThread::EmitWakeupSignal()
{
}
