//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTTHREADPOOL_HPP
#define MUTENET_IOCONTEXTTHREADPOOL_HPP

#include <SharedPool.hpp>
#include <Runnable.hpp>
#include <Event.hpp>

namespace EventLoop
{
	using WorkFunctor = std::function<void ( )>;
	using WorkPendingQueue = concurrency::concurrent_queue<WorkFunctor>;
	
	class IOContextThreadPool
	{
		Util::Event _TriggerEvent;
		bool _ClearFlag = false;
		
		std::vector<std::thread> _ThreadPool;
		WorkPendingQueue _Queue;
	public:
		IOContextThreadPool ( ) = default;
		
		IOContextThreadPool ( const int32_t WorkerCount );
		
		~IOContextThreadPool ( );
		
		void EnqueueJob ( const WorkFunctor &&Functor );
		
		size_t GetWorkerThreadCount ( ) const;
	
	private:
		void InnerWork ( );
	};
}


#endif //MUTENET_IOCONTEXTTHREADPOOL_HPP
