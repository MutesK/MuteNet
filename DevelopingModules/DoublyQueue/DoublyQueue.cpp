#include "DoublyQueue.h"


template <typename Data>
DoublyQueue<Data>::DoublyQueue()
{
}

template <typename Data>
DoublyQueue<Data>::~DoublyQueue()
{
}

template <typename Data>
void DoublyQueue<Data>::SwapQueues()
{
	SAFE_UNIQUELOCK(_lock);
	
	std::swap(_forwardQueue, _backwardQueue);
}

template <typename Data>
void DoublyQueue<Data>::Put(const Data& data)
{
	SAFE_SHAREDLOCK(_lock);

	_forwardQueue.emplace(data);
}

template <typename Data>
void DoublyQueue<Data>::Get(Data& OUT _data)
{
	SAFE_SHAREDLOCK(_lock);

	_data = _backwardQueue.front();
	_backwardQueue.pop();
}
