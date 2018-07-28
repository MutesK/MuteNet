#include "stdafx.h"
#include "Task.h"
#include "Thread.h"
#include "TaskAgent.h"
#include "CLockFreeQueue.h"

TaskAgent::TaskAgent()
	:_timeoutSec(0), _currenthangcheck(0), _prevhangcheck(0)
{
}


TaskAgent::~TaskAgent()
{
}

void TaskAgent::AddTask(Task t)
{
	_requestQueue.Enqueue(&t);
}

Task TaskAgent::DequeCompletedTask()
{
	Task t;

	_resultQueue.Dequeue(&t);

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
	_requestQueue.ClearQueue();
	_resultQueue.ClearQueue();
}

void TaskAgent::DoWork()
{
	Task task;

	while (this->isThreadRunning())
	{
		while (_requestQueue.Dequeue(&task))
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
	
}
