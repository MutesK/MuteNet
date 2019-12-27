#include "MuteNetDB.h"
#include "DBCommand.h"
#include "DBResultSet.h"

namespace Util
{
	namespace DB
	{
		DBCommand::DBCommand(const std::shared_ptr<DBConnection>& pConnection, const std::string& Query)
			:_pResultSet(nullptr),
			_pConnection(pConnection),
			_Query(Query)
		{
		}
		DBCommand::~DBCommand()
		{
		}

		void DBCommand::Prepare()
		{
			PrepareImpl();

			_pResultSet = std::shared_ptr<DBResultSet>(NewResultSetImpl());
		}

		void DBCommand::Reset()
		{
			ResetImpl();
		}




	}
}