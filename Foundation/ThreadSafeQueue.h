#pragma once

#include <queue>
#include <memory>

namespace Util
{
	template <class T>
	class ThreadSafeQueue
	{
	private:
		std::queue<T>			_queue;
		std::shared_mutex		_mutex;

	public:
		void Enqueue(const T& data);
		void Enqueue(const std::shared_ptr<T>& data);
		void Dequeue();
		size_t Size();
		T& Top();

		ThreadSafeQueue Copy(ThreadSafeQueue& Queue)
		{
			{
				std::unique_lock<std::shared_mutex> lock(Queue._mutex);
				_queue = Queue._queue;
			}

			return *this;
		}

		GET_SET_ATTRIBUTE(std::shared_mutex, mutex);
	};

	template<class T>
	inline void ThreadSafeQueue<T>::Enqueue(const T& data)
	{
		std::unique_lock<std::shared_mutex> lock(_mutex);

		_queue.push(data);
	}


	template<class T>
	inline void ThreadSafeQueue<T>::Enqueue(const std::shared_ptr<T>& data)
	{
		std::unique_lock<std::shared_mutex> lock(_mutex);

		_queue.push(*data);
	}



	template<class T>
	inline void ThreadSafeQueue<T>::Dequeue()
	{
		std::unique_lock<std::shared_mutex> lock(_mutex);

		_queue.pop();
	}

	template<class T>
	inline T& ThreadSafeQueue<T>::Top()
	{
		std::shared_lock<std::shared_mutex> lock(_mutex);

		return _queue.front();
	}

	template<class T>
	inline size_t ThreadSafeQueue<T>::Size()
	{
		std::shared_lock<std::shared_mutex> lock(_mutex);

		return _queue.size();
	}
}

