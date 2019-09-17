#pragma once

#include "ScheduleTask.h"

namespace Util
{
	class Example : public Module::ScheduleTask
	{
	public:
		Example(const Timestamp& time, const uint32_t RepeatCount=1);
		~Example();

		virtual void DoWork() override;
	};
}