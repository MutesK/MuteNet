#include "MuteNetDB.h"
#include "DBResultSet.h"

namespace Util
{
	namespace DB
	{
		void DBResultSet::GetNextResultSet()
		{
			if (!NextResultSetImpl())
			{
				throw;
			}
		}

		void DBResultSet::GetNext()
		{
			if (!NextImpl())
			{
				throw;
			}
		}
		void DBResultSet::Skip()
		{
			while(NextResultSetImpl()) {}
		}
	}
}
