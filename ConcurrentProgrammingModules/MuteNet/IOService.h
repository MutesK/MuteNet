#pragma once

#include "../ASyncQueue/ASyncQueue.h"
#include "Link.h"

namespace Network
{
	class IOService final : public Util::ASyncQueue
	{
	public:
		IOService() = default;

		bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout);
		void Stop();

		void RegisterSocket(void* handle, LinkPtr Context);
	private:
		NON_COPYABLE(IOService);

		virtual void HandleCompletion(const uint32_t workerIndex, ULONG_PTR Context, LPOVERLAPPED Overlapped) override;
		virtual void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR Context) override;
	};

	inline bool IOService::Initialize(const uint32_t numberOfWorkers, const uint32_t timeout)
	{
		return ASyncQueue::Initialize(numberOfWorkers, timeout);
	}

	inline void IOService::Stop()
	{
		ASyncQueue::Stop();
	}

	inline void IOService::RegisterSocket(void* handle, LinkPtr Context)
	{
		CreateIoCompletionPort(handle, _iocpHandle, reinterpret_cast<ULONG_PTR>(&Context), 0);
	}

	inline void IOService::HandleCompletion(const uint32_t workerIndex, ULONG_PTR Context, LPOVERLAPPED Overlapped)
	{
		auto Link = reinterpret_cast<LinkPtr *>(Context);


	}

	inline void IOService::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR Context)
	{
		auto Link = reinterpret_cast<LinkPtr*>(Context);


	}
}