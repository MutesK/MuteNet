<<<<<<< HEAD
//#pragma once
//
//class TaskAgent : public Thread
//{
//public:
//	TaskAgent();
//	~TaskAgent();
//
//
//	void AddTask(Task t);
//	Task DequeCompletedTask();
//
//	void CheckHang();
//	void Flush();
//
//	// void SetMessageDispatcher();
//protected:
//	virtual void DoWork() override;
//	virtual void EmitWakeupSignal() override;
//
//private:
//	size_t				 _timeoutSec;
//
//	//CLockFreeQueue<Task> _requestQueue;
//	//CLockFreeQueue<Task> _resultQueue;
//
//};
//
=======
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

>>>>>>> 0d419a24dc84e66035c257b7fdf0f25396484262
