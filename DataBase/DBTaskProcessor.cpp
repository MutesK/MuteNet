#include "MuteNetDB.h"
#include "DBTaskProcessor.h"
#include "DBConnection.h"
#include "DBConnectionService.h"
#include "DBTask.h"
#include "DBResultSet.h"
#include "DBCommand.h"

namespace Util
{
	namespace DB
	{
		DBTaskProcessor::DBTaskProcessor(const std::string& DBName)
			:_DBName(DBName)
		{
		}

		bool DBTaskProcessor::Init(DBConnectionService* pService, uint32_t numOfGroup)
		{
			_DBConnections.resize(numOfGroup, nullptr);

			for (uint32_t index = 0; index < numOfGroup; ++index)
			{
				const auto pConnection = pService->AcquireConnection();  // Factory·Î ÀüÈ¯

				if (nullptr == pConnection)
				{
					return false;
				}

				_DBConnections[index] = pConnection;
			}

			return true;
		}

		void DBTaskProcessor::WarpUp()
		{
			for (const auto pConnection : _DBConnections)
			{
				pConnection->Close();
			}

			_DBConnections.clear();
		}

		void DBTaskProcessor::Process(const uint32_t WorkerIndex, DBTask* pTask)
		{
			const auto pConnection = _DBConnections[WorkerIndex];

			if (nullptr == pConnection)
			{
				throw;
			}

			ProcessImpl(WorkerIndex, pConnection, pTask);
		}

		void DBTaskProcessor::Timeout(const uint32_t WorkerIndex)
		{
			const auto pConnection = _DBConnections[WorkerIndex];

			if (nullptr != pConnection)
			{
				if (!pConnection->KeepAlive())
				{
					pConnection->Close();
				}
			}
		}

		void DBTaskProcessor::ProcessImpl(const uint32_t WorkerIndex, const std::shared_ptr<DBConnection>& pConnection, DBTask* const pTask)
		{
			// Exception Start P
			auto pCommand = pConnection->FindOutDBCommand(pTask);

			do 
			{
				pTask->BindInput(pCommand);

				int16_t result;
				const auto pResultSet = pCommand->Execute();
				pResultSet->GetResult(result);

				if (pResultSet->NextResultSetImpl())
				{
					pTask->BindOutput(pResultSet);
					pResultSet->BindImpl();
				}
				else
				{
					pResultSet->Skip();
				}
				

				pTask->ProcessResult(result, pResultSet);
				pCommand->Reset();

			} while (pTask->Next());
		}
	}
}