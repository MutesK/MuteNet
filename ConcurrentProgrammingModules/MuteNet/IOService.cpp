#include "IOService.h"
#include "IOContext.h"

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

	void IOService::HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped,
		DWORD TransfferedBytes)
	{
		IOContext* pContext = reinterpret_cast<IOContext*>(Overlapped);

		if(nullptr == pContext)
		{
			// Logger
			return;
		}

		pContext->Callback(pContext, TransfferedBytes, reinterpret_cast<void *>(CompletionKey));
	}

	void IOService::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
	{
	}
}
