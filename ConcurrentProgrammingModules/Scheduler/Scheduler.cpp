#include "Scheduler.h"
#include "ScheduleTask.h"
#include <future>

using namespace Module;
using namespace std::chrono;

Scheduler::Scheduler()
{
	SetShutdownThreadSignal(false);
	_schedulerThread = std::make_unique<std::thread>(std::bind(&Scheduler::DoWork, this));
}

Scheduler::~Scheduler()
{	
	SetShutdownThreadSignal(true);
	_schedulerThread->join();
}

void Scheduler::RegisterTask(ScheduleTask* pTask)
{
	if (pTask == nullptr)
		return;

	{
		std::lock_guard<std::mutex> lock(_queueMutex);

		_taskQueue.insert(std::make_pair(pTask->_startime, pTask));
	}
}

void Scheduler::UnRegisterTask(ScheduleTask* pTask)
{
	if (pTask == nullptr)
		return;

	std::lock_guard<std::mutex> lock(_queueMutex);

	_taskQueue.erase(pTask->_startime);
}

void Scheduler::DoWork()
{
	ScheduleTask* pTask = nullptr;

	while(true)
	{
		if (this->GetShutdownThreadSignal())
			break;

		while (!_taskQueue.empty())
		{
			pTask = _taskQueue.begin()->second;

			if (pTask->_startime >= high_resolution_clock::now())
				continue;

			std::future<void> fut = std::async(std::bind(&ScheduleTask::Process, pTask));

			if (!pTask->isRepeated())
			{
				std::lock_guard<std::mutex> lock(_queueMutex);

				_taskQueue.erase(pTask->_startime);
			}
		}

		std::this_thread::yield();
	}
}
