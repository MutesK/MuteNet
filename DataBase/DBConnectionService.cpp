#include "MuteNetDB.h"
#include "DBConnectionService.h"

#include "DBConnection.h"


namespace Util
{
	namespace DB
	{
		DBConnectionService::DBConnectionService(const uint32_t NumOfCommand, DBConnectionSetting* const pConnectionSetting)
			:_NumOfCommands(NumOfCommand), _pConnectionSetting(pConnectionSetting)
		{
		}

		DBConnection* DBConnectionService::AcquireConnection()
		{
			auto pConnection = AcquireConnectionImpl();

			if (nullptr == pConnection)
			{

			}

			return pConnection;
		}

		void DBConnectionService::ReleaseConnection(DBConnection* const pConnection)
		{
			delete pConnection;
		}

		DBConnection* DBConnectionService::AcquireConnectionImpl()
		{
			const auto pConnection = NewConnection();

			if (pConnection == nullptr)
			{
				return nullptr;
			}


			if (!pConnection->Connect())
			{

			}

			return pConnection;

		}
	}
}