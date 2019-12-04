#include "pch.h"
#include "Task.h"
#include "TaskAgent.h"


namespace Util
{
	TaskAgent::TaskAgent(const std::string& agentName)
		: _TimeoutSec(0), _CurrentHangCheck(0), _PrevHangCheck(0), super(agentName)
	{
	}

	TaskAgent::~TaskAgent()
	{
		Finalize();
	}

	void TaskAgent::AddTask(const Task& t)
	{
		_RequestQueue.push(t);

		SetEvent();
	}

	Task TaskAgent::DequeCompletedTask()
	{
		Task t;

		while (!_ResultQueue.try_pop(t))
		{ }

		return t;
	}

	void TaskAgent::CheckHang(bool& OUT hang)
	{
		hang = false;

		if (_CurrentHangCheck != _PrevHangCheck)
			hang = true;

		_PrevHangCheck = _CurrentHangCheck;
	}

	void TaskAgent::Flush()
	{
		_RequestQueue.clear();
		_ResultQueue.clear();
	}

	void TaskAgent::DoWork()
	{
		Task task;

		while (_RequestQueue.try_pop(task))
		{
			++_CurrentHangCheck;

			task.Do();

			if (task.IsEmptyCompleteLambda())
			{
				task.CompleteDo();
			}

			++_PrevHangCheck;
		}
	}
}