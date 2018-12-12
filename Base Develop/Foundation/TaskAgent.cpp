
#include <thread>
#include "Event.h"
#include "Task.h"
#include "TaskAgent.h"


TaskAgent::TaskAgent(const std::string& agentName)
	:_timeoutSec(0), _currenthangcheck(0), _prevhangcheck(0)
{
	_threadstoprequest = false;

	SetThreadName(agentName);
	Start();
}


TaskAgent::~TaskAgent()
{
}

void TaskAgent::AddTask(Task t)
{
	_requestQueue.push(t);
}

Task TaskAgent::DequeCompletedTask()
{
	Task t;

	while (!_resultQueue.try_pop(t));

	return t;
}

void TaskAgent::CheckHang(bool& OUT hang)
{
	hang = false;

	if (_currenthangcheck != _prevhangcheck)
		hang = true;

	_prevhangcheck = _currenthangcheck;
}

void TaskAgent::Flush()
{
	_requestQueue.clear();
	_resultQueue.clear();
}

void TaskAgent::DoWork()
{
	Task task;

	while (!_threadstoprequest)
	{
		_event.WaitSignal();

		while (_requestQueue.try_pop(task))
		{
			++_currenthangcheck;

			task.Do();

			if (false == task.IsEmptyCompleteLambda())
			{
				task.CompleteDo();
			}

			++_prevhangcheck;
		}
	}
	
}

void TaskAgent::EmitWakeupSignal()
{
	_threadstoprequest = true;

	_event.SetEvent();
}
