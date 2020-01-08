#include "ServerApplication.h"


int main()
{
	try
	{
		Application App;
		App.Listen(6000);

		while (true)
		{
			std::this_thread::yield();
		}
	}
	catch (Exception & e)
	{
		LogHelper::Log(ELogLevel::Fatal, e.what());
	}

	return 0;
}