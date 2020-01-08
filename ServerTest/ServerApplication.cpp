#include "Precompile.h"
#include "ServerApplication.h"

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

	///	_testInterface.CreateTemperClass();
	// _testInterface.InsertTemperClass("Hello, I'm MuteNet Lib");
	_testInterface.GetTemperClass();
}

void Application::Listen(const uint16_t Port)
{
	_ServerHandlePtr = Network::Listen(Port, _ListenCallback);
}
