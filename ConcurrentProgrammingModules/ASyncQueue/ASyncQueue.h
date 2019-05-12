#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace Module
{
	class ASyncQueue abstract : private Noncopyable
	{
	public:
		ASyncQueue();
		virtual ~ASyncQueue();

	protected:
		bool Initialize(const uint32_t numberOfWorkers, const uint32_t timeout);
		void Stop();
		bool PostQueue(ULONG_PTR Pointer);

		void Run(const uint32_t workerIndex);

		virtual void HandleCompletion(const uint32_t workerIndex) = 0;
		virtual void HandleTimeout(const uint32_t WorkerIndex) =0;

	private:
		HANDLE				  _iocpHandle;
		std::atomic<uint32_t> _workerIndex;
		std::vector<std::thread*> _workers;
		uint32_t				  _timeout;
	};

}