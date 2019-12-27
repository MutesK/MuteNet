#pragma once

namespace Util
{
	namespace DB
	{
		class DBConnectionService;
		class DBCommand;
		class DBTask;
		class DBCommandPool;
		class DBConnection abstract
		{
		protected:
			DBConnectionService* _pService;
			std::shared_ptr<DBCommandPool> _pDBCommandPool;
		public:
			DBConnection(DBConnectionService* const pService);
			virtual ~DBConnection();

			bool IsConnected();
			bool Connect();
			bool KeepAlive();
			void Close();

			void BeginTransaction();
			void CommitTransaction();
			void RollbackTransaction();

			virtual std::shared_ptr<DBCommand> FindOutDBCommand(const DBTask* const pDBTask);

			// Automatically Managed Raw Pointer,
			// Don't Delete it
			virtual DBCommand* Prepare(const DBTask* const pDBTask) = 0;

		protected:
			virtual bool IsConnectedImpl() = 0;
			virtual bool ConnectImpl() = 0;
			virtual void CloseImpl() = 0;
			virtual bool KeepAliveImpl() = 0;
			virtual void RefreshImpl () = 0;

			virtual void BeginTransactionImpl() = 0;
			virtual void CommitTransactionImpl() = 0;
			virtual void RollbackTransactionImpl() = 0;
		};
	}
}