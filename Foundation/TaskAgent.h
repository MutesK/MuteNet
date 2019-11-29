#pragma once

namespace Util
{
	class Task;
	class TaskAgent final
	{
		size_t _TimeoutSec;

		size_t _CurrentHangCheck;
		size_t _PrevHangCheck;

		std::unique_ptr<std::thread>		_Thread;

		Concurrency::concurrent_queue<Task> _ResultQueue;
		Concurrency::concurrent_queue<Task> _RequestQueue;
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