#pragma once

namespace Util
{
	namespace DB
	{
		class DBCommand;
		class DBResultSet;
		class DBTask abstract
		{
			uint32_t _id;   // Task ID
			uint32_t _dbNameIndex;  //  연동할 데이터베이스 인덱스
			std::string _name;   // Task 이름
			std::string _dbCommandName;  // Query
			std::string _keyPrefix;

		public:
			DBTask(const uint32_t id, const uint32_t dbNameIndex,
				const std::string& name, const std::string& dbCommandName,
				const std::string& keyPrefix);
			virtual ~DBTask() = default;

			virtual void BindInput(std::shared_ptr<DBCommand> pCommand) = 0;
			virtual void BindOutput(std::shared_ptr<DBResultSet> pResultSet) = 0;
			virtual bool Next() = 0;
			virtual bool IsLast() const = 0;

			std::string GetDBCommandName() const;

			GET_CONST_ATTRIBUTE(uint32_t, id);
			GET_CONST_ATTRIBUTE(uint32_t, dbNameIndex);
		protected:
			virtual void ProcessResult(const uint32_t Result,
				const std::shared_ptr<DBResultSet> pResultSet) = 0;
			virtual void HandleError(const uint32_t Result) = 0;

			friend class DBTaskProcessor;
		};

		inline std::string DBTask::GetDBCommandName() const
		{
			return _dbCommandName;
		}
	}
}