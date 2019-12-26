#pragma once

namespace Util
{
	namespace DB
	{
		class DBCommand;
		class DBTask;
		class DBConnection;
		class DBCommandPool final
		{
			std::vector<DBCommand*> _Dictionary;

		public:
			DBCommandPool(const uint32_t numOfCommand);
			~DBCommandPool();

			DBCommand* FindOutDBCommand(const DBTask* const pTask, DBConnection* const pConnection);
		};
	}
}

