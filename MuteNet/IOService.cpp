#include "pch.h"
#include "IOService.h"
#include "IOContext.h"
#include "../Foundation/MiniDump.h"

namespace Network
{
	bool IOService::Initialize(const uint32_t numberOfWorkers, const uint32_t timeout)
	{
		WSADATA wsadata;
		if (0 != WSAStartup(MAKEWORD(2, 2), &wsadata))
			return false;

		return ASyncQueue::Initialize(numberOfWorkers, timeout);
	}

	void IOService::Stop()
	{
		WSACleanup();
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


		switch (pContext->Type)
		{
		case IOType::IO_ACCEPT:
			reinterpret_cast<AcceptContext*>(pContext)->IOComplete(TransfferedBytes, 
				reinterpret_cast<void *>(CompletionKey), this);
			break;
		case IOType::IO_CONNECT:
			reinterpret_cast<ConnectContext*>(pContext)->IOComplete(TransfferedBytes,
				reinterpret_cast<void*>(CompletionKey));
			break;
		case IOType::IO_RECV:
			reinterpret_cast<RecvContext*>(pContext)->IOComplete(TransfferedBytes,
				reinterpret_cast<void*>(CompletionKey));
			break;
		case IOType::IO_SEND:
			reinterpret_cast<SendContext*>(pContext)->IOComplete(TransfferedBytes,
				reinterpret_cast<void*>(CompletionKey));
			break;
		default:
			break;
		}
	}

	void IOService::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
	{
	}
}
