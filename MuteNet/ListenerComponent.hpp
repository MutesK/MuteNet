//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_LISTENERCOMPONENT_HPP
#define MUTENET_LISTENERCOMPONENT_HPP

#include "Descriptor.h"
#include <Runnable.hpp>

namespace EventLoop
{
	class IDescriptor;
	using DescriptorPtr = IDescriptor*;
	
	class IOContextImpl;
	
    class ListenerComponent : public IDescriptor
	{
	public:
		using CallbackDelegate = std::function<void (ListenerComponent *pRawListener, DescriptorPtr clientsocket,
                                                     sockaddr* address, int length, void *Self )>;
	protected:
		CallbackDelegate _ListenCallbackDelegate;
		void *_Self;
		
		friend class IOContextImpl;
	
	public:
		virtual ~ListenerComponent ( );

    protected:
		ListenerComponent ( const RawIOContextImplPtr &ContextEvent,
		                    CallbackDelegate &&Callback,
		                    void *Self, descriptor_t listenSocket );
	};
}

#endif //MUTENET_LISTENERCOMPONENT_HPP
