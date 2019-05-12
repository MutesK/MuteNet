#include "Example.h"
#include "Scheduler.h"

using namespace Module;
using namespace std::chrono_literals;

Scheduler scheduler;

int main()
{
	scheduler.RegisterTask(dynamic_cast<ScheduleTask*>(&Example(std::chrono::steady_clock::now() + 10s, 10)));

	while(1)
	{ }
}