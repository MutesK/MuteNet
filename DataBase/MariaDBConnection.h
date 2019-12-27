#pragma once

#include "DBConnection.h"

namespace Util
{
	namespace DB
	{
		class MariaDBConnection : public DBConnection
		{
			typedef DBConnection super;
			MYSQL _mySQL;
		public:
			MariaDBConnection(DBConnectionService* const pService);
			virtual ~MariaDBConnection();

			std::shared_ptr<MYSQL_STMT> CreateStatement();

			virtual DBCommand* Prepare(const DBTask* const pDBTask) override;
			virtual std::shared_ptr<DBCommand> FindOutDBCommand(const DBTask* const pDBTask);

			virtual bool IsConnectedImpl() override;
			virtual bool ConnectImpl() override;
			virtual void CloseImpl() override;
			virtual bool KeepAliveImpl() override;
			virtual void RefreshImpl() override;
			virtual void BeginTransactionImpl() override;
			virtual void CommitTransactionImpl() override;
			virtual void RollbackTransactionImpl() override;
		};
	}
}