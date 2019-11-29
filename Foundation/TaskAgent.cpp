#include "pch.h"
#include "Task.h"
#include "TaskAgent.h"

namespace Util
{
	TaskAgent::TaskAgent(const std::string& agentName)
		: _TimeoutSec(0), _CurrentHangCheck(0), _PrevHangCheck(0)
	{
		_Thread = std::make_unique<std::thread>(std::bind(&TaskAgent::DoWork, this));

		const auto threadhandle = _Thread->native_handle();

		ChangeThreadName(threadhandle, agentName);
	}

	TaskAgent::~TaskAgent()
	{
		_Thread->join();
	}

	void TaskAgent::AddTask(const Task& t)
	{
		_RequestQueue.push(t);
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

		while (true)
		{
			while (_RequestQueue.try_pop(task))
			{
				++_CurrentHangCheck;

				task.Do();

				if (!task.IsEmptyCompleteLambda())
				{
					task.CompleteDo();
				}

				++_PrevHangCheck;
			}
		}
	}
}