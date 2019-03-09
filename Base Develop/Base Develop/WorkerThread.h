#pragma once

#include "framework.h"

class WorkerThread final
{
public:
	WorkerThread(HANDLE iocp);
	~WorkerThread();

	void Stop();
protected:
	void DoWork();
private:
	HANDLE _hIOCP;
	std::list<std::thread> workers;
};


