#pragma once

#include "DBTask.h"

namespace Util
{
	namespace DB
	{
		class DBCommand;
		class DBResultSet;
		class TestDBTask : public DBTask
		{
			typedef DBTask super;
			const int binddata = 45;
			const std::string stringdata = "Hello, I'm MuteNet Lib";
		public:
			TestDBTask();
			virtual ~TestDBTask() = default;

			// DBTask을(를) 통해 상속됨
			virtual void BindInput(std::shared_ptr<DBCommand> pCommand) override;
			virtual void BindOutput(std::shared_ptr<DBResultSet> pResultSet) override;
			virtual bool Next() override;
			virtual bool IsLast() const override;

			virtual void ProcessResult(const uint32_t Result, const std::shared_ptr<DBResultSet> pResultSet) override;
			virtual void HandleError(const uint32_t Result) override;
		};
	}
}