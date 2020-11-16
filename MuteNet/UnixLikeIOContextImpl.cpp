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
    IUnixLikeIOContextImpl::CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag,
                                           int backlog, socket_t listenSocket)
    {
        return ListenerPtr ((ListenerComponent *)new LikeUnixListenerComponent(this, std::move(Callback),
                Self, Flag, backlog, listenSocket));
    }

    DescriptorPtr IUnixLikeIOContextImpl::CreateSocket(socket_t Socket)
    {
        return new UnixLikeDescriptor(this, Socket);
    }
}