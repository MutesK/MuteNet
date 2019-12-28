#include "ServerApplication.h"
#include "Logger.h"
#include "TaskManager.h"
#include "MariaDBConnectionService.h"
#include "DBConnectionSetting.h"
#include "DBTaskImpl.h"

using namespace Util::DB;

class ConsoleLogListener : public Logger::Listener
{
public:
	virtual void Log(const std::string logFmt, ELogLevel level) override
	{
		std::cout << logFmt << std::endl;
	}
};

Application::Application()
	:_ListenCallback(std::make_shared<ListenCallback>())
{
	Util::Logger::Get().AttachListener(new ConsoleLogListener());

	Util::Logger::Get().StartRun();
	Util::TaskManager::Get(); // Just Called Constructor

	DBConnectionSetting Setting;

	Setting._Host = "127.0.0.1";
	Setting._Port = 5000;
	Setting._User = "root";
	Setting._Passwd = "!@#wnsals640803";
	Setting._DBNameIndex = 0;
	Setting._DBName = "mutenettest";

	DB::MariaDBConnectionService Service(10, &Setting);

	_DBASyncQueue.Initialize(10, INFINITE);
	_DBASyncQueue.AddDBProcessor(&Service);


	TestDBTask* Task = new TestDBTask;
	DBTaskWorkSerializedQueue* Queue = new DBTaskWorkSerializedQueue;

	Queue->push(Task);
	_DBASyncQueue.EnqueueDBTask(Queue);
}

void Application::Listen(const uint16_t Port)
{
	_ServerHandlePtr = Network::Listen(Port, _ListenCallback);
}
