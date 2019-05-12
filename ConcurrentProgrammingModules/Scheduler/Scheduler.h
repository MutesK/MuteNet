#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include <map>
#include <mutex>

/*
	Task 등록 및 해제
	타이머에 의해 시작됨

	Task
		- Work Function 
		- 시작지점 EventTimeStamp
*/
namespace Module
{
	class ScheduleTask;

	class Scheduler final
	{
	public:
		Scheduler();
		~Scheduler();

		void RegisterTask(ScheduleTask* pTask);
		void UnRegisterTask(ScheduleTask* pTask);

		NON_COPYABLE(Scheduler);
		THREAD_SHUTDOWN_FLAG();
	private:
		void DoWork();
	private:
		std::map<Timestamp, ScheduleTask*> _taskQueue;
		std::mutex _queueMutex;
		std::unique_ptr<std::thread> _schedulerThread;
	};
}
