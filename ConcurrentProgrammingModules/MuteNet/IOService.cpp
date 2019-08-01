#include "IOService.h"
#include "Acceptor.h"

namespace Network
{
	bool IOService::Initialize(const uint32_t numberOfWorkers, const uint32_t timeout)
	{
		return ASyncQueue::Initialize(numberOfWorkers, timeout);
	}

	void IOService::Stop()
	{
		ASyncQueue::Stop();
	}

	void IOService::HandleCompletion(const uint32_t workerIndex, ULONG_PTR Context, LPOVERLAPPED Overlapped,
		DWORD TransfferedBytes)
	{
	}

	void IOService::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR Context)
	{
	}
}
