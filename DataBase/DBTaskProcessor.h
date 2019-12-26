#pragma once

namespace Util
{
	namespace DB
	{
		class DBConnectionService;
		class DBTask;
		class DBConnection;
		class DBTaskProcessor final
		{
			std::string _DBName;
			std::vector<DBConnection*> _DBConnections;
		public:
			DBTaskProcessor(const std::string& DBName);

			bool Init(DBConnectionService* pService, uint32_t numOfGroup);
			void WarpUp();

			void Process(const uint32_t WorkerIndex, DBTask* pTask);
			void Timeout(const uint32_t WorkerIndex);
		private:
			void ProcessImpl(const uint32_t WorkerIndex,
				DBConnection* const pConnection,
				DBTask* const pTask);

		};
	}
}
