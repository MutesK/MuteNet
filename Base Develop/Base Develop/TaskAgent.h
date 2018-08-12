#pragma once

#include "Thread.h"

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
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;

private:
	size_t				 _timeoutSec;

	size_t				_currenthangcheck;
	size_t				_prevhangcheck;

	bool				_threadstoprequest;

	Concurrency::concurrent_queue<Task> _resultQueue;
	Concurrency::concurrent_queue<Task> _requestQueue;
};


