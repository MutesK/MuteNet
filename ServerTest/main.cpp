#include "ServerApplication.h"


int main()
{
	ServerApplication App;
	App.Listen(25000);
	
	while(true)
	{
		std::this_thread::yield();
	}

	return 0;
}