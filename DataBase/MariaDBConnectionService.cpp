#include "MuteNetDB.h"
#include "MariaDBConnectionService.h"
#include "MariaDBConnection.h"
namespace Util
{
	namespace DB
	{
		MariaDBConnectionService::MariaDBConnectionService(const uint32_t numOfCommand, DBConnectionSetting* const pConnectionSetting)
			:super(numOfCommand, pConnectionSetting)
		{
		}


		DBConnection* MariaDBConnectionService::NewConnection()
		{
			return new MariaDBConnection(this);
		}
	}
}