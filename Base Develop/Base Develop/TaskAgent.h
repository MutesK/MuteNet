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
