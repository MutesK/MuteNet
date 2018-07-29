#pragma once

#include "Thread.h"
#include "ThreadSafeQueue.h"

class Task;
class TaskAgent : public Thread
{
public:
	TaskAgent();
	~TaskAgent();


	void AddTask(Task t);
	Task DequeCompletedTask();

	void CheckHang(bool& OUT hang);
	void Flush();

	// void SetMessageDispatcher();
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;

private:
	size_t				 _timeoutSec;

	size_t				_currenthangcheck;
	size_t				_prevhangcheck;

	ThreadSafeQueue<Task> _resultQueue;
	ThreadSafeQueue<Task> _requestQueue;
};

