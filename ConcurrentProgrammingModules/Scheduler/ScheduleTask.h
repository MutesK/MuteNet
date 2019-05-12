#pragma once


#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace Module
{
	class ScheduleTask abstract
	{
	public:
		ScheduleTask(const Timestamp& time, const uint32_t RepeatCount = 1);
		virtual ~ScheduleTask();

	private:
		void Process();
		bool isRepeated() const;
	private:
		bool operator()(ScheduleTask* left, ScheduleTask* right) const
		{
			return left->_time > right->_time;
		}

		NON_COPYABLE(ScheduleTask);
	protected:
		virtual void DoWork()=0;

		friend class Scheduler;
	private:
		uint32_t _repeatcount;
		Timestamp _time;
	};
}