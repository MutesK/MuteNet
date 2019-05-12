#include "Example.h"

namespace Module
{
	Example::Example(const Timestamp& time, const uint32_t RepeatCount)
		:ScheduleTask(time, RepeatCount)
	{

	}


	Example::~Example()
	{
	}

	void Example::DoWork()
	{
		std::cout << "ScheduleTask is Work" << std::endl;
	}

}