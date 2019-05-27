#include "ScheduleTask.h"

using namespace Module;
using namespace std::chrono_literals;



ScheduleTask::ScheduleTask(const Timestamp& time, const uint32_t RepeatCount)
	:_repeatcount(RepeatCount), _startime(time)
{
}


ScheduleTask::~ScheduleTask()
{
}

void ScheduleTask::Process()
{
	if (_repeatcount <= 0)
		return;

	DoWork();

	--_repeatcount;
}

bool ScheduleTask::isRepeated() const
{
	return _repeatcount > 0;
}
