#pragma once

#include "SocketIO.h"

namespace MuteNet
{

	class ServiceListener : public Util::ASyncQueue
	{
	protected:
		ExtensionFunctions  _extension;
	public:
		ServiceListener();
		bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout) override;
		void Stop() override;

		void RegisterHandle(void* handle, void* CompletionKey) const;

		void HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) override;
		void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey) override;

		ExtensionFunctions* GetExtension();
	};

	inline void ServiceListener::RegisterHandle(void* handle, void* CompletionKey) const
	{
		CreateIoCompletionPort(handle, _iocpHandle, reinterpret_cast<ULONG64>(CompletionKey), 0);
	}

}
