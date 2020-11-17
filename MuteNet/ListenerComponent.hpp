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

        virtual void Read() override;

        virtual void Write(void *data, size_t length) override;

        virtual void Enable() override;

        virtual void Disable(uint16_t Flag) override;
    protected:
        virtual bool _Read() override;

        virtual bool _Write() override;

    protected:
		ListenerComponent ( const RawIOContextImplPtr &ContextEvent,
		                    CallbackDelegate &&Callback,
		                    void *Self, descriptor_t listenSocket );
	};
}

#endif //MUTENET_LISTENERCOMPONENT_HPP
