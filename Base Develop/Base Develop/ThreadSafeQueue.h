#pragma once

#include "stdafx.h"

template <class TYPE>
class ThreadSafeQueue
{
public:
	ThreadSafeQueue()
	{	
	}
	~ThreadSafeQueue()
	{
	}

	void Enqueue(TYPE& IN data)
	{
		UNIQUE_LOCK(_mutex);
		_queue.push(data);
	}

	bool Dequeue(TYPE& OUT data)
	{
		UNIQUE_LOCK(_mutex);
		data = _queue.front();
		_queue.pop();

		return true;
	}

	void ClearQueue()
	{
		UNIQUE_LOCK(_mutex);
		while (!_queue.empty())
			_queue.pop();
	}


private:
	std::shared_mutex	_mutex;
	std::queue<TYPE>	_queue;
};

