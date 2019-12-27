#include "MuteNetDB.h"
#include "DBCommandPool.h"
#include "DBTask.h"
#include "DBConnection.h"
#include "DBCommand.h"
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

		std::shared_ptr<DBCommand> DBCommandPool::FindOutDBCommand(const DBTask* const pTask,
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
				pCommand = std::shared_ptr<DBCommand>(pConnection->Prepare(pTask));

				_Dictionary[taskid] = pCommand;
			}

			return pCommand;
		}
	}
}
