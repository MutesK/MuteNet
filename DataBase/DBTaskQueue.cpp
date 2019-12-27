#include "DBTaskQueue.h"
#include "DBTaskProcessor.h"
#include "DBConnectionService.h"
#include "DBTask.h"


namespace Util
{
	namespace DB
	{
		bool DBTaskQueue::Initialize(const uint32_t numOfWorker, const uint32_t KeepAlivePeriod)
		{
			return super::Initialize(numOfWorker, KeepAlivePeriod);
		}

		void DBTaskQueue::Stop()
		{
			super::Stop();

			_DBTaskProcessor.clear();
		}

		bool DBTaskQueue::AddDBProcessor(DBConnectionService* pService)
		{
			auto pDBProcessor = std::make_shared<DBTaskProcessor>(pService->GetDBName());

			if (nullptr == pDBProcessor)
			{
				return false;
			}

			if (false == pDBProcessor->Init(pService, static_cast<uint32_t>(_Workers.size())))
			{
				return false;
			}

			const auto dbNameIndex = pService->GetDBNameIndex();

			if (_DBTaskProcessor.size() <= dbNameIndex)
			{
				_DBTaskProcessor.resize(static_cast<size_t>(dbNameIndex) + 1, nullptr);
			}

			_DBTaskProcessor[dbNameIndex] = pDBProcessor;

			return true;

		}

		bool DBTaskQueue::EnqueueDBTask(DBTaskWorkSerializedQueue* const pSerializedQueue)
		{
			return super::PostQueue(reinterpret_cast<ULONG_PTR>(pSerializedQueue));
		}

		void DBTaskQueue::HandleCompletion(const uint32_t workerIndex, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes)
		{
			auto pSerializedQueue = reinterpret_cast<DBTaskWorkSerializedQueue *>(CompletionKey);

			do 
			{
				auto pTask = pSerializedQueue->front();
				
				const auto dbNameIndex = pTask->get_dbNameIndex();

				if (_DBTaskProcessor.size() < dbNameIndex)
				{
					return;
				}

				const auto pDBProcessor = _DBTaskProcessor[dbNameIndex];

				if (nullptr != pDBProcessor)
				{
					pDBProcessor->Process(workerIndex, pTask);
				}

				pSerializedQueue->pop();

			} while (pSerializedQueue->size());
		}

		void DBTaskQueue::HandleTimeout(const uint32_t WorkerIndex, ULONG_PTR CompletionKey)
		{
			for (auto pDBTaskProcessor : _DBTaskProcessor)
			{
				if (nullptr != pDBTaskProcessor)
				{
					pDBTaskProcessor->Timeout(WorkerIndex);
				}
			}

		}

		void DBTaskQueue::HandleError(const uint32_t workerIndex, DWORD Error, ULONG_PTR CompletionKey, LPOVERLAPPED Overlapped, DWORD TransfferedBytes)
		{
		}
	}
}