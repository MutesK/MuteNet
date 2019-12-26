#pragma once

namespace Util
{
	namespace DB
	{
		class DBCommand;
		class DBResultSet;
		class DBTask abstract
		{
			uint32_t _id;
			uint32_t _dbNameIndex;
			std::string _name;
			std::string _dbCommandName;
			std::string _keyPrefix;

		public:
			DBTask(const uint32_t id, const uint32_t dbNameIndex,
				const std::string& name, const std::string& dbCommandName,
				const std::string& keyPrefix);
			virtual ~DBTask() = default;

			virtual void BindInput(DBCommand* const pCommand) = 0;
			virtual void BindOutput(DBResultSet* const pResultSet) = 0;
			virtual bool Next() = 0;
			virtual bool IsLast() const = 0;

			GET_CONST_ATTRIBUTE(uint32_t, id);
			GET_CONST_ATTRIBUTE(uint32_t, dbNameIndex);
		protected:
			virtual void ProcessResult(const uint32_t Result,
				DBResultSet* const pResultSet) = 0;
			virtual void HandleError(const uint32_t Result) = 0;

			friend class DBTaskProcessor;
		};
	}
}