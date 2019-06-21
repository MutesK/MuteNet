#include "IOService.h"
#include "IOContext.h"
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
		auto pContext = reinterpret_cast<IOContext*>(Overlapped);

		switch (pContext->_Type)
		{
		case OverlappedType::eConnected:
			return;
		case OverlappedType::eDisconnected:
			return;
		case OverlappedType::eAccepted:
		{
			auto pAcceptor = reinterpret_cast<Acceptor*>(Context);
			pAcceptor->ProcessAccept();
		}
		return;
		case OverlappedType::eRecived:
			return;
		case OverlappedType::eSended:
			return;

		}
	}

	void IOService::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR Context)
	{
	}
}
