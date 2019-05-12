#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include <map>
#include <mutex>

/*
	Task ��� �� ����
	Ÿ�̸ӿ� ���� ���۵�

	Task
		- Work Function 
		- �������� EventTimeStamp
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
