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
			DBConnectionService* _pService;
			DBCommandPool* _pDBCommandPool;
		public:
			DBConnection(DBConnectionService* const pService);
			virtual ~DBConnection();

			bool IsConnected();
			bool Connect();
			bool KeepAlive();
			void Close();

			virtual DBCommand* FindOutDBCommand(const DBTask* const pDBTask);
			virtual DBCommand* Prepare(const DBTask* const pDBTask) = 0;

		protected:
			virtual bool IsConnectedImpl() = 0;
			virtual bool ConnectImpl() = 0;
			virtual void CloseImpl() = 0;
			virtual bool KeepAliveImpl() = 0;
			virtual void RefreshImpl () = 0;
		};
	}
}