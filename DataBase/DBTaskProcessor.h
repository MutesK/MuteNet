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
			std::vector<std::shared_ptr<DBConnection>> _DBConnections;
		public:
			DBTaskProcessor(const std::string& DBName);

			bool Init(DBConnectionService* pService, uint32_t numOfGroup);
			void WarpUp();

			void Process(const uint32_t WorkerIndex, DBTask* pTask);
			void Timeout(const uint32_t WorkerIndex);
		private:
			void ProcessImpl(const uint32_t WorkerIndex,
				const std::shared_ptr<DBConnection>& pConnection,
				DBTask* const pTask);

		};
	}
}
