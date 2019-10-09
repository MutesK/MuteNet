#include "pch.h"
#include "ServiceListener.h"
#include "IOContext.h"
#include "../Foundation/MiniDump.h"
#include "Link.h"
#include "Acceptor.h"


namespace MuteNet
{
	bool ServiceListener::Initialize(const uint32_t numberOfWorkers, const uint32_t timeout)
	{
		return ASyncQueue::Initialize(numberOfWorkers, timeout);
	}

	void ServiceListener::Stop()
	{
		ASyncQueue::Stop();
	}

	void ServiceListener::HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped,
		DWORD TransfferedBytes)
	{
		IOContext* pContext = reinterpret_cast<IOContext*>(Overlapped);
		const auto LinkPtr = pContext->linkPtr;

		if(nullptr == pContext || nullptr == LinkPtr)
		{
			// Logger
			return;
		}
	}

	void ServiceListener::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
	{
	}
}
