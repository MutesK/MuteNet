//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IoContextThreadPool.hpp"

namespace EventLoop
{

	void IOContextThreadPool::InnerWork()
	{
		WorkFunctor Functor;

		while (!_ClearFlag)
		{
			if (_Queue.empty())
			{
				continue;
			}

			if (!_Queue.try_pop(Functor))
			{
				continue;
			}

			Functor();
		}
	}

	IOContextThreadPool::IOContextThreadPool(const int32_t WorkerCount)
	{
		_ThreadPool.resize(WorkerCount);

		for (int index = 0; index < WorkerCount; ++index)
		{
			_ThreadPool[index] = std::thread([&]()
				{
					InnerWork();
				});
		}
	}

	IOContextThreadPool::~IOContextThreadPool()
	{
		_ClearFlag = true;
		_TriggerEvent.SetAll();

		std::for_each(_ThreadPool.begin(), _ThreadPool.end(), [](std::thread& thread)
			{
				thread.join();
			});
	}

	void IOContextThreadPool::EnqueueJob(const WorkFunctor&& Functor)
	{
		_Queue.push(Functor);

		_TriggerEvent.Set();
	}

	size_t IOContextThreadPool::GetWorkerThreadCount() const
	{
		return _ThreadPool.size();
	}


}
