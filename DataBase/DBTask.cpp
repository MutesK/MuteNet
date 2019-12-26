#include "MuteNetDB.h"
#include "DBTask.h"


namespace Util
{
	namespace DB
	{
		DBTask::DBTask(const uint32_t id, const uint32_t dbNameIndex, const std::string& name, const std::string& dbCommandName, const std::string& keyPrefix)
			:_id(id),
			_dbNameIndex(dbNameIndex),
			_name(name),
			_dbCommandName(dbCommandName),
			_keyPrefix(keyPrefix)
		{
		}
	}
}