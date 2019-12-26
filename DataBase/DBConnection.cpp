#include "MuteNetDB.h"
#include "DBConnection.h"
#include "DBConnectionService.h"
#include "DBCommandPool.h"

namespace Util
{
	namespace DB
	{
		DBConnection::DBConnection(DBConnectionService* const pService)
			:_pService(pService)
		{
			_pDBCommandPool = new DBCommandPool(pService->GetNumOfCommand());
		}

		DBConnection::~DBConnection()
		{
			delete _pDBCommandPool;
		}

		bool DBConnection::IsConnected()
		{
			if (nullptr == _pService)
			{
				return false;
			}

			return IsConnectedImpl();
		}

		bool DBConnection::Connect()
		{
			Close();

			return ConnectImpl();
		}

		bool DBConnection::KeepAlive()
		{
			if (!IsConnected())
			{
				if (!Connect())
				{
					return false;
				}

				RefreshImpl();
			}

			return KeepAliveImpl();
		}

		void DBConnection::Close()
		{
			CloseImpl();
		}

		DBCommand* DBConnection::FindOutDBCommand(const DBTask* const pDBTask)
		{
			return _pDBCommandPool->FindOutDBCommand(pDBTask, this);
		}
	}
}
