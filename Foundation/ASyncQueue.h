#pragma once

#include "foundation.h"

namespace Util
{
	class ASyncQueue abstract
	{
	protected:
		HANDLE						_IocpHandle;
		std::atomic<uint32_t>		_WorkerIndex;
		std::vector<std::thread*>	_Workers;
		uint32_t					_Timeout;
	public:
		ASyncQueue();
		virtual ~ASyncQueue();

	protected:
		virtual bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout);
		virtual void Stop();
		virtual bool PostQueue(ULONG_PTR Pointer);

		void Run(const uint32_t workerIndex);

		virtual void HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) = 0;
		virtual void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey) =0;
		virtual void HandleError(const uint32_t workerIndex, DWORD Error, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) = 0;
	};

}