#include "MuteNetDB.h"
#include "DBTaskImpl.h"
#include "MariaDBCommand.h"
#include "MariaDBResultSet.h"
#include "LogHelper.h"

using namespace Util::DB;

TestDBTask::TestDBTask()
	:super(0, 0, "TestDBTask", "INSERT INTO `test`.`test` VALUES (?);", "")
{
}

void TestDBTask::BindInput(std::shared_ptr<DBCommand> pCommand)
{
	pCommand->Bind(4.51210);
}

void TestDBTask::BindOutput(std::shared_ptr<DBResultSet> pResultSet)
{
}

bool TestDBTask::Next()
{
	return false;
}

bool TestDBTask::IsLast() const
{
	return false;
}

void TestDBTask::ProcessResult(const uint32_t Result, const std::shared_ptr<DBResultSet> pResultSet)
{
	LogHelper::Log(ELogLevel::Debug, "Task Complete");
}

void TestDBTask::HandleError(const uint32_t Result)
{
}
