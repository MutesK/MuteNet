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
		const auto ExtOverlappedPtr = reinterpret_cast<ExtentedOverlapped *>(Overlapped);
		ASyncRequest* RequestPtr = reinterpret_cast<ASyncRequest*>(ExtOverlappedPtr->SelfPtr);

		if(RequestPtr)
			RequestPtr->IOCompletion(TransfferedBytes);
	}

	void ServiceListener::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
	{
	}

	void ServiceListener::HandleError(const uint32_t workerIndex, DWORD Error, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes)
	{
		const auto ExtOverlappedPtr = reinterpret_cast<ExtentedOverlapped*>(Overlapped);
		ASyncRequest* RequestPtr = reinterpret_cast<ASyncRequest*>(ExtOverlappedPtr->SelfPtr);

		if (RequestPtr)
			RequestPtr->IOCompletion(Error);
	}
}
