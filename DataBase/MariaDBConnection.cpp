#include "MuteNetDB.h"
#include "MariaDBConnection.h"
#include "DBConnectionService.h"
#include "DBCommandPool.h"
#include "MariaDBCommand.h"
#include "DBTask.h"

namespace Util
{
	namespace DB
	{
		MariaDBConnection::MariaDBConnection(DBConnectionService* const pService)
			:super(pService)
		{
		}

		MariaDBConnection::~MariaDBConnection()
		{
			Close();
		}

		std::shared_ptr<MYSQL_STMT> MariaDBConnection::CreateStatement()
		{
			const auto pStmt = ::mysql_stmt_init(&_mySQL);

			if (nullptr == pStmt)
			{
				throw;
			}

			return std::shared_ptr<MYSQL_STMT>(pStmt);
		}

		DBCommand* MariaDBConnection::Prepare(const DBTask* const pDBTask)
		{
			const auto pStmt = CreateStatement();
			const auto pCommand = new MariaDBCommand(this, pStmt, pDBTask->GetDBCommandName());

			try
			{
				pCommand->Prepare();
			}
			catch (std::exception e)
			{
			}

			return pCommand;
		}

		std::shared_ptr<DBCommand> MariaDBConnection::FindOutDBCommand(const DBTask* const pDBTask)
		{
			const auto pCommand = super::FindOutDBCommand(pDBTask);

			if (nullptr == pCommand)
			{
				return nullptr;
			}

			return pCommand;
		}

		bool MariaDBConnection::IsConnectedImpl()
		{
			const auto result = ::mysql_ping(&_mySQL);

			return result == 0;
		}

		bool MariaDBConnection::ConnectImpl()
		{
			if (nullptr == ::mysql_init(&_mySQL))
			{
				return false;
			}

			my_bool reconnect = false;
			::mysql_options(&_mySQL, MYSQL_OPT_RECONNECT, &reconnect);

			uint32_t connectionTimeout = 3;
			::mysql_options(&_mySQL, MYSQL_OPT_CONNECT_TIMEOUT, &connectionTimeout);

			std::string charSet = "utf8";
			::mysql_options(&_mySQL, MYSQL_SET_CHARSET_NAME, charSet.c_str());

			const auto pSetting = _pService->GetSetting();
			const auto pMySQL = ::mysql_real_connect(
				&_mySQL,
				pSetting->_Host.c_str(),
				pSetting->_User.c_str(),
				pSetting->_Passwd.c_str(),
				pSetting->_db.c_str(),
				pSetting->_Port,
				nullptr,
				CLIENT_MULTI_RESULTS);

			if (nullptr == pMySQL)
			{
				const auto errNo = mysql_errno(&_mySQL);

				CloseImpl();
				return false;
			}

			return true;
		}

		void MariaDBConnection::CloseImpl()
		{
			::mysql_close(&_mySQL);
		}

		bool MariaDBConnection::KeepAliveImpl()
		{
			const auto result = ::mysql_ping(&_mySQL);

			return 0 == result;
		}

		void MariaDBConnection::RefreshImpl()
		{
			if (nullptr != _pDBCommandPool)
			{
				for (const auto pCommand : *(_pDBCommandPool.get()))
				{
					std::dynamic_pointer_cast<MariaDBCommand>(pCommand)->Refresh();
				}
			}
		}

		void MariaDBConnection::BeginTransactionImpl()
		{
			::mysql_query(&_mySQL, "START TRANSACTION;");
		}

		void MariaDBConnection::CommitTransactionImpl()
		{
			::mysql_query(&_mySQL, "COMMIT;");
		}

		void MariaDBConnection::RollbackTransactionImpl()
		{
			::mysql_query(&_mySQL, "ROLLBACK;");
		}
	}
}