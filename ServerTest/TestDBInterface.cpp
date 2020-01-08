#include "TestDBInterface.h"
#include "DBTask.h"
#include "DBCommand.h"
#include "MariaDBResultSet.h"

using namespace Util;

namespace App
{
	TestDBInterface::TestDBInterface()
		:super("MariaDB", "127.0.0.1", 12000, "root", "!ss640803",
			"test", new DataBase::DBTaskQueue())
	{
		_TaskQueue->Initialize(10, INFINITE);

		Initialize();
	}


	void TestDBInterface::CreateTemperClass()
	{
		//class CreateDBTask : public DataBase::DBTask
		//{
		//public:
		//	CreateDBTask(COMMON_DBTASK_ARGUMENT)
		//		:COMMON_DBTASK_CALLCTOR
		//	{}

		//	virtual void BindInput(std::shared_ptr<DataBase::DBCommand> pCommand) override
		//	{
		//	}
		//	virtual void BindOutput(std::shared_ptr<DataBase::DBResultSet> pResultSet) override
		//	{
		//	}

		//	virtual bool Next() override
		//	{
		//		return false;
		//	}

		//	virtual bool IsLast() const override
		//	{
		//		return false;
		//	}

		//	virtual void ProcessResult(const std::shared_ptr<DataBase::DBResultSet> pResultSet) override
		//	{
		//		LogHelper::Log(ELogLevel::Debug, "CreateDBTask Task Complete");
		//	}

		//	virtual void HandleError(const uint32_t Result) override
		//	{
		//	}

		//};

		//const auto Task = super::CreateDBTask<CreateDBTask>("CreateDBTask",
		//	"CREATE TABLE `?` ("
		//	"`TemperStr` VARCHAR(50) NULL DEFAULT NULL);");

		//super::ProcessASyncDB(Task);
	}


	void TestDBInterface::InsertTemperClass(const std::string& InsertData)
	{
		class InsertTemper : public DataBase::DBTask
		{
		public:
			InsertTemper(COMMON_DBTASK_ARGUMENT) : COMMON_DBTASK_CALLCTOR {}
			std::string temp;

			virtual void BindInput(std::shared_ptr<DataBase::DBCommand> pCommand) override
			{
				pCommand->Bind(temp.c_str(), temp.length());
			}
			virtual void BindOutput(std::shared_ptr<DataBase::DBResultSet> pResultSet) override
			{
			}

			virtual bool Next() override
			{
				return false;
			}

			virtual bool IsLast() const override
			{
				return false;
			}

			virtual void ProcessResult(const std::shared_ptr<DataBase::DBResultSet> pResultSet) override
			{
				LogHelper::Log(ELogLevel::Debug, "CreateDBTask Task Complete");
			}

			virtual void HandleError(const uint32_t Result) override
			{
			}

		};

		const auto Task = super::CreateDBTask<InsertTemper>("InsertTemper",
			"INSERT INTO `test table` (`TemperStr`) VALUES (?);");

		Task->temp = InsertData;

		super::ProcessASyncDB(Task);
	}

	void TestDBInterface::GetTemperClass()
	{
		class GetTemp : public DataBase::DBTask
		{
		public:
			char temp[5000]{0};
			uint32_t receviedtemplength{0};



			GetTemp(COMMON_DBTASK_ARGUMENT) : COMMON_DBTASK_CALLCTOR{}

			virtual void BindInput(std::shared_ptr<DataBase::DBCommand> pCommand) override
			{
			}
			virtual void BindOutput(std::shared_ptr<DataBase::DBResultSet> pResultSet) override
			{
				pResultSet->Bind(temp, receviedtemplength, 5000);
			}

			// 입력 값 셋
			virtual bool Next() override
			{
				return false;
			}

			// 입력 값 셋
			virtual bool IsLast() const override
			{
				return true;
			}

			virtual void ProcessResult(const std::shared_ptr<DataBase::DBResultSet> pResultSet) override
			{
			}

			virtual void HandleError(const uint32_t Result) override
			{
			}

		};

		const auto Task = super::CreateDBTask<GetTemp>("SelectTemper",
			"SELECT * FROM `test table`;");

		super::ProcessASyncDB(Task);
	}

}