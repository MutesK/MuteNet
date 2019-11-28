#pragma once

namespace Util
{
	class Task;
	class TaskAgent final
	{
		size_t _timeoutSec;

		size_t _currentHangCheck;
		size_t _prevHangCheck;

		std::unique_ptr<std::thread>		_thread;

		Concurrency::concurrent_queue<Task> _resultQueue;
		Concurrency::concurrent_queue<Task> _requestQueue;
	public:
		TaskAgent(const std::string& agentName);
		~TaskAgent();

		void AddTask(const Task& t);
		Task DequeCompletedTask();

		void CheckHang(bool& OUT hang);
		void Flush();
	protected:
		void DoWork();
	private:
		NON_COPYABLE(TaskAgent);
	};

}