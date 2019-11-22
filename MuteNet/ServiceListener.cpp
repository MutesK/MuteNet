#include "pch.h"
#include "ServiceListener.h"
#include "Link.h"
#include "ASyncRequest.h"


namespace MuteNet
{
	ServiceListener::ServiceListener()
	{
		InitExtensionFunctions(&_extension);
	}

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
		auto Request = reinterpret_cast<ASyncRequest*>(Overlapped);

		Request->IOCompletion(TransfferedBytes);
	}

	void ServiceListener::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
	{
	}
}
