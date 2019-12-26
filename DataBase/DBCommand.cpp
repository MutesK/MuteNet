#include "MuteNetDB.h"
#include "DBCommand.h"
#include "DBResultSet.h"

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
			delete _pResultSet;
		}

		void DBCommand::Prepare()
		{
			PrepareImpl();

			_pResultSet = NewResultSetImpl();
		}

		void DBCommand::Reset()
		{
			ResetImpl();
		}




	}
}