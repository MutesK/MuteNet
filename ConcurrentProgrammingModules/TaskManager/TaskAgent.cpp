#include <thread>
#include "Task.h"
#include "TaskAgent.h"

namespace Util
{
	TaskAgent::TaskAgent(const std::string& agentName)
		: _timeoutSec(0), _currentHangCheck(0), _prevHangCheck(0)
	{
		_thread = std::make_unique<std::thread>(std::bind(&TaskAgent::DoWork, this));

		const auto threadhandle = _thread->native_handle();

		ChangeThreadName(threadhandle, agentName);
	}

	TaskAgent::~TaskAgent()
	{
		SetShutdownThreadSignal(true);
		_thread->join();
	}

	void TaskAgent::AddTask(Task t)
	{
		_requestQueue.push(t);
	}

	Task TaskAgent::DequeCompletedTask()
	{
		Task t;

		while (!_resultQueue.try_pop(t))
		{ }

		return t;
	}

	void TaskAgent::CheckHang(bool& OUT hang)
	{
		hang = false;

		if (_currentHangCheck != _prevHangCheck)
			hang = true;

		_prevHangCheck = _currentHangCheck;
	}

	void TaskAgent::Flush()
	{
		_requestQueue.clear();
		_resultQueue.clear();
	}

	void TaskAgent::DoWork()
	{
		Task task;

		while (!GetShutdownThreadSignal())
		{
			while (_requestQueue.try_pop(task))
			{
				++_currentHangCheck;

				task.Do();

				if (!task.IsEmptyCompleteLambda())
				{
					task.CompleteDo();
				}

				++_prevHangCheck;
			}
		}
	}
}