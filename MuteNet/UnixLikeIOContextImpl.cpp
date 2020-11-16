//
// Created by JunMin Kim on 20. 11. 15..
//


#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeIOContextImpl.hpp"
#include "LikeUnixListenerComponent.h"
#include "UnixLikeSocketDescriptor.hpp"

namespace EventLoop
{

    IUnixLikeIOContextImpl::IUnixLikeIOContextImpl(IOContextEvent &Event, const uint32_t NumOfWorkerThread,
                                                   const uint32_t Timeout) :
            IOContextImpl(Event, NumOfWorkerThread, Timeout)
    {
    }

    IUnixLikeIOContextImpl::ContextContainer &IUnixLikeIOContextImpl::GetContextContainer()
    {
        return _Container;
    }

    ListenerPtr
    IUnixLikeIOContextImpl::CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag,
                                           int backlog, socket_t listenSocket)
    {
        return ListenerPtr ((ListenerComponent *)new LikeUnixListenerComponent(this, std::move(Callback),
                Self, Flag, backlog, listenSocket));
    }

    SocketPtr IUnixLikeIOContextImpl::CreateSocket(socket_t Socket)
    {
        return new UnixLikeSocketDescriptor(this, Socket);
    }
}