#include "pch.h"
#include "IOService.h"
#include "IOContext.h"
#include "../Foundation/MiniDump.h"
#include "Link.h"
#include "Acceptor.h"
#include "EngineIO.h"

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
		const auto LinkPtr = pContext->linkPtr;

		if(nullptr == pContext || nullptr == LinkPtr)
		{
			// Logger
			return;
		}


		switch (pContext->Type)
		{
		case IOType::IO_ACCEPT:
		{
			auto acceptor = reinterpret_cast<Acceptor*>(CompletionKey);

			acceptor->AcceptCompleteIO();
		}
			break;
		case IOType::IO_CONNECT:
			EngineIO::OnConnected(LinkPtr);
			break;
		case IOType::IO_RECV:
			LinkPtr->RecvCompleteIO(TransfferedBytes,
				reinterpret_cast<void*>(CompletionKey));
			break;
		case IOType::IO_SEND:
			LinkPtr->SendCompleteIO(TransfferedBytes,
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
