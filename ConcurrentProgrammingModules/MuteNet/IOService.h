#pragma once

#include "../ASyncQueue/ASyncQueue.h"

namespace Network
{
	class IOService : public Util::ASyncQueue
	{
	public:
		IOService() = default;
		bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout) override;
		void Stop() override;

		void RegisterHandle(void* handle, void* Context) const;

		void HandleCompletion(const uint32_t workerIndex, ULONG_PTR Context, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) override;
		void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR Context) override;
	};

	inline void IOService::RegisterHandle(void* handle, void* Context) const
	{
		CreateIoCompletionPort(handle, _iocpHandle, reinterpret_cast<ULONG64>(Context), 0);
	}

}
