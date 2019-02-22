#pragma once

#include "../Foundation/foundation.h"
#include "../Foundation/Thread.h"
#include "Session.h"

class WorkerThread final : public Thread
{
public:
	WorkerThread(HANDLE iocp);
	~WorkerThread();
protected:
	virtual void DoWork() override;
	virtual void EmitWakeupSignal() override;
private:
	HANDLE _hIOCP;
};

