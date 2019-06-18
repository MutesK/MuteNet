#include "ASyncQueue.h"


using namespace Util;


ASyncQueue::ASyncQueue()
	:_iocpHandle(nullptr), _workerIndex(0), _timeout(INFINITE)
{
}


ASyncQueue::~ASyncQueue()
{
}


bool ASyncQueue::Initialize(const uint32_t numberOfWorkers, const uint32_t timeout)
{
	if (numberOfWorkers == 0)
	{
		return false;
	}


	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (_iocpHandle == nullptr)
	{
		return false;
	}

	_timeout = timeout;

	for (uint32_t i = 0; i < numberOfWorkers; ++i)
	{
		_workers.push_back(new std::thread(std::bind(&ASyncQueue::Run, this, i)));
		ChangeThreadName(_workers[i]->native_handle(), "WorkerThread");
	}

	return true;
}

void ASyncQueue::Stop()
{
	if (_iocpHandle == nullptr)
		return;

	for (auto thread : _workers)
	{
		PostQueue(0);
	}

	for (auto worker : _workers)
	{
		worker->join();

		delete worker;
	}

	_workers.clear();
	CloseHandle(_iocpHandle);
	_iocpHandle = nullptr;
}

bool ASyncQueue::PostQueue(ULONG_PTR Pointer)
{
	if (PostQueuedCompletionStatus(_iocpHandle, 0, Pointer, nullptr) == false)
	{
		return false;
	}

	return true;
}

void ASyncQueue::Run(const uint32_t workerIndex)
{
	DWORD byteTransferred = 0;
	ULONG_PTR CompletionKey = 0;
	LPOVERLAPPED lpOverlapped = nullptr;


	while (true)
	{
		if (GetQueuedCompletionStatus(_iocpHandle, &byteTransferred,
			&CompletionKey, &lpOverlapped, _timeout) == false)
		{
			const auto lastError = GetLastError();
			
			if (WAIT_TIMEOUT != lastError)
			{
				break;
			}

			HandleTimeout(workerIndex, CompletionKey);
			continue;
		}

		HandleCompletion(workerIndex, CompletionKey, lpOverlapped);
	}
}