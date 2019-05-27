#include "DoublyQueue.h"



DoublyQueue::DoublyQueue()
{
}


DoublyQueue::~DoublyQueue()
{
}

void DoublyQueue::SwapQueues()
{
	SAFE_UNIQUELOCK(_lock);
	
	std::swap(_forwardQueue, _backwardQueue);
}

void DoublyQueue::Put(const std::string& data)
{
	SAFE_SHAREDLOCK(_lock);

	_forwardQueue.emplace(data);
}

void DoublyQueue::Get(std::string& _data)
{
	SAFE_SHAREDLOCK(_lock);

	_data = _backwardQueue.front();
	_backwardQueue.pop();
}
