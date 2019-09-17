#pragma once

namespace Util
{
	class ASyncQueue abstract
	{
	public:
		ASyncQueue();
		virtual ~ASyncQueue();

	protected:
		virtual bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout);
		virtual void Stop();
		bool PostQueue(ULONG_PTR Pointer);

		void Run(const uint32_t workerIndex);

		virtual void HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) = 0;
		virtual void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey) =0;

	protected:
		HANDLE				  _iocpHandle;
		std::atomic<uint32_t> _workerIndex;
		std::vector<std::thread*> _workers;
		uint32_t				  _timeout;
	};

}