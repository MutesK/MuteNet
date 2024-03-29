#ifndef MUTENET_TASKAGENT_H_
#define MUTENET_TASKAGENT_H_

#include "Runnable.hpp"

namespace Util
{
	class Task;

	class ThreadHolder;

	class TaskAgent final : public Runnable
	{
		typedef ThreadHolder super;
	private:
		size_t _TimeoutSec;

		size_t _CurrentHangCheck;
		size_t _PrevHangCheck;

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
		virtual void DoWork() override;

	private:
		NON_COPYABLE(TaskAgent);
	};

}

#endif