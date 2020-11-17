//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTIMPL_HPP
#define MUTENET_IOCONTEXTIMPL_HPP

#include "IoContextThreadPool.hpp"
#include "ListenerComponent.hpp"
#include "Descriptor.h"

#include <Runnable.hpp>

namespace EventLoop
{
	class IOContextEvent;
	
	class ListenerComponent;
	using ListenerPtr = std::shared_ptr<ListenerComponent>;
	
	class IDescriptor;
	using DescriptorPtr = IDescriptor*;
	
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
		                                     void *Self, descriptor_t listenSocket ) = 0;
		
		virtual DescriptorPtr CreateDescriptor (descriptor_t descriptor) = 0;
		
		std::shared_ptr<IOContextThreadPool> &GetThreadPool ( );
	};
}


#endif //MUTENET_IOCONTEXTIMPL_HPP
