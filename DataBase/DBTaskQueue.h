#pragma once

#include "MuteNetDB.h"

namespace Util
{
	namespace DB
	{
		class DBConnectionService;
		class DBTaskProcessor;
		class DBTask;

		typedef std::queue<DBTask*> DBTaskWorkSerializedQueue;

		class DBTaskQueue : public ASyncQueue
		{
			typedef ASyncQueue super;

			std::vector<std::shared_ptr<DBTaskProcessor>> _DBTaskProcessor;
		public:
			virtual bool Initialize(const uint32_t numOfWorker, const uint32_t KeepAlivePeriod);
			void Stop();

			bool AddDBProcessor(DBConnectionService* pService);

			bool EnqueueDBTask(DBTaskWorkSerializedQueue* const pSerializedQueue);

			// ASyncQueue을(를) 통해 상속됨
			virtual void HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, 
				LPOVERLAPPED Overlapped, DWORD TransfferedBytes) override;
			virtual void HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey) override;
			virtual void HandleError(const uint32_t workerIndex, DWORD Error, 
				ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes) override;
		};
	}
}