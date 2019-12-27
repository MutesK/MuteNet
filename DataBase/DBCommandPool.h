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
			std::vector<std::shared_ptr<DBCommand>> _Dictionary;

		public:
			DBCommandPool(const uint32_t numOfCommand);
			~DBCommandPool();

			const std::vector<std::shared_ptr<DBCommand>>::iterator begin();
			const std::vector<std::shared_ptr<DBCommand>>::iterator end();


			std::shared_ptr<DBCommand> FindOutDBCommand(const DBTask* const pTask, DBConnection* const pConnection);
		};

		inline const std::vector<std::shared_ptr<DBCommand>>::iterator DBCommandPool::begin()
		{
			return _Dictionary.begin();
		}

		inline const std::vector<std::shared_ptr<DBCommand>>::iterator DBCommandPool::end()
		{
			return _Dictionary.end();
		}


	}
}

