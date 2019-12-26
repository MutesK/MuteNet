#include "MuteNetDB.h"
#include "DBCommandPool.h"
#include "DBTask.h"
#include "DBConnection.h"

namespace Util
{
	namespace DB
	{
		DBCommandPool::DBCommandPool(const uint32_t numOfCommand)
		{
			_Dictionary.resize(numOfCommand);
		}

		DBCommandPool::~DBCommandPool()
		{
			_Dictionary.clear();
		}

		DBCommand* DBCommandPool::FindOutDBCommand(const DBTask* const pTask,
			DBConnection* const pConnection)
		{
			auto taskid = pTask->get_id();

			if (_Dictionary.size() <= taskid)
			{
				throw;
			}

			auto pCommand = _Dictionary[taskid];

			if (nullptr == pCommand)
			{
				pCommand = pConnection->Prepare(pTask);

				_Dictionary[taskid] = pCommand;
			}

			return pCommand;
		}
	}
}
