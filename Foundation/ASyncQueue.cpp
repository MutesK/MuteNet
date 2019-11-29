#include "pch.h"
#include "ASyncQueue.h"
#include "../MuteNet/SocketUtil.h"


using namespace Util;


ASyncQueue::ASyncQueue()
	:_IocpHandle(nullptr), _WorkerIndex(0), _Timeout(INFINITE)
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


	_IocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
	if (_IocpHandle == nullptr)
	{
		return false;
	}

	_Timeout = timeout;

	for (uint32_t i = 0; i < numberOfWorkers; ++i)
	{
		_Workers.push_back(new std::thread(std::bind(&ASyncQueue::Run, this, i)));
		ChangeThreadName(_Workers[i]->native_handle(), "WorkerThread");
	}

	return true;
}

void ASyncQueue::Stop()
{
	if (_IocpHandle == nullptr)
		return;

	for (auto thread : _Workers)
	{
		PostQueue(0);
	}

	for (auto worker : _Workers)
	{
		worker->join();

		delete worker;
	}

	_Workers.clear();
	CloseHandle(_IocpHandle);
	_IocpHandle = nullptr;
}

bool ASyncQueue::PostQueue(ULONG_PTR Pointer)
{
	if (PostQueuedCompletionStatus(_IocpHandle, 0, Pointer, nullptr) == false)
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
		if (GetQueuedCompletionStatus(_IocpHandle, &byteTransferred,
			&CompletionKey, &lpOverlapped, _Timeout) == false)
		{
			const auto lastError = WSAGetLastError();
			
			if (WAIT_TIMEOUT != lastError)
			{
				HandleError(workerIndex, lastError, CompletionKey, lpOverlapped, byteTransferred);
				continue;
			} 

			HandleTimeout(workerIndex, CompletionKey);
			continue;
		}
		else
		{
			if (byteTransferred == 0 && CompletionKey == 0 && lpOverlapped == nullptr)
			{
				PostQueue(0);
				return;
			}

			HandleCompletion(workerIndex, CompletionKey, lpOverlapped, byteTransferred);
		}
	}
}