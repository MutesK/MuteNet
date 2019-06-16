#pragma once

#include "../ConcurrentProgrammingModules/CommonHeader.h"

namespace Util
{
	class Task;
	class TaskAgent final
	{
	public:
		TaskAgent(const std::string& agentName);
		~TaskAgent();

		void AddTask(Task t);
		Task DequeCompletedTask();

		void CheckHang(bool& OUT hang);
		void Flush();

		THREAD_SHUTDOWN_FLAG();
	protected:
		void DoWork();
	private:
		NON_COPYABLE(TaskAgent);
	private:
		size_t _timeoutSec;

		size_t _currentHangCheck;
		size_t _prevHangCheck;

		std::unique_ptr<std::thread>		_thread;

		Concurrency::concurrent_queue<Task> _resultQueue;
		Concurrency::concurrent_queue<Task> _requestQueue;
	};

}