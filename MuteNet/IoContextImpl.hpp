//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTIMPL_HPP
#define MUTENET_IOCONTEXTIMPL_HPP

#include "IoContextThreadPool.hpp"
#include "ListenerComponent.hpp"
#include <Runnable.hpp>

namespace EventLoop
{
	class IOContextEvent;
	
	class ListenerComponent;
	
	using ListenerPtr = std::shared_ptr<ListenerComponent>;
	
	class ISocketDescriptor;
	
	using SocketPtr = std::shared_ptr<ISocketDescriptor>;
	
	class IOContextImpl
	{
	protected:
		IOContextEvent &_Event;
		std::shared_ptr<IOContextThreadPool> _ThreadPool;  // Using For Notify Completion
		uint32_t _Timeout;
	public:
		IOContextImpl ( IOContextEvent &Event,
		                const uint32_t NumOfWorkerThread, const uint32_t Timeout );
		
		virtual ListenerPtr CreateListener ( ListenerComponent::CallbackDelegate &&Callback,
		                                     void *Self, uint32_t Flag, int backlog, socket_t listenSocket ) = 0;
		
		virtual SocketPtr CreateSocket ( socket_t Socket ) = 0;
		
		std::shared_ptr<IOContextThreadPool> &GetThreadPool ( );
	};
}


#endif //MUTENET_IOCONTEXTIMPL_HPP
