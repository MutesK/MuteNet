#pragma once

/*
 *	Thread-Safe Double Queue
 *  락의 요소를 최대한 줄이기 위해 전방 1 큐는 Only Write, 
 *  후방 큐는 Only Read한다.
 *  
 *  일정 시점이 되면 둘의 Container는 Swap하며 이때만, 락이 걸린다.
 *  Thread A는 Write만 하고, Thread B는 Read하여 처리
 *  
 *  테스트를 위해 Generic하지 않고, std::string 으로 테스트한다.
 */

#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include "../DeadLock Checker/SafeSharedLock.h"
#include <queue>

class DoublyQueue final
{
public:
	DoublyQueue();
	~DoublyQueue();

	void SwapQueues();

	void Put(const std::string& _data);
	void Get(std::string& OUT _data);
private:
	SafeSharedLock _lock;

	std::queue<std::string> _forwardQueue;
	std::queue<std::string> _backwardQueue;
};

