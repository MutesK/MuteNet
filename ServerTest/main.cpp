#include "ServerApplication.h"


int main()
{
	Application App;
	App.Listen(6000);
	
	while(true)
	{
		std::this_thread::yield();
	}

	return 0;
}