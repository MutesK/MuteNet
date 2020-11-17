//
// Created by JunMin Kim on 20. 11. 15..
//


#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeIOContextImpl.hpp"
#include "LikeUnixListenerComponent.h"
#include "UnixLikeDescriptor.hpp"

namespace EventLoop
{

    IUnixLikeIOContextImpl::IUnixLikeIOContextImpl(IOContextEvent &Event, const uint32_t NumOfWorkerThread,
                                                   const uint32_t Timeout) :
            IOContextImpl(Event, NumOfWorkerThread, Timeout)
    {
    }

    ListenerPtr
    IUnixLikeIOContextImpl::CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, 
        descriptor_t listenSocket)
    {
        return ListenerPtr ((ListenerComponent *)new LikeUnixListenerComponent(this, std::move(Callback),
                Self, listenSocket));
    }

    DescriptorPtr IUnixLikeIOContextImpl::CreateDescriptor(descriptor_t descriptor)
    {
        return new UnixLikeDescriptor(this, descriptor);
    }
}