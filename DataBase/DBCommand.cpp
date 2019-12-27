#include "MuteNetDB.h"
#include "DBCommand.h"
#include "DBResultSet.h"
#include "DBConnection.h"

namespace Util
{
	namespace DB
	{
		DBCommand::DBCommand(DBConnection* const pConnection, const std::string& Query)
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

		void DBCommand::BeginTransaction()
		{
			_pConnection->BeginTransaction();
		}

		void DBCommand::CommitTransaction()
		{
			_pConnection->CommitTransaction();
		}

		void DBCommand::RollbackTransaction()
		{
			_pConnection->RollbackTransaction();
		}




	}
}