#pragma once

#include "ScheduleTask.h"

namespace Module
{
	class Example : public ScheduleTask
	{
	public:
		Example(const Timestamp& time, const uint32_t RepeatCount=1);
		~Example();

		virtual void DoWork() override;
	};
}