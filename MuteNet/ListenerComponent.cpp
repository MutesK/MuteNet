//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "ListenerComponent.hpp"
#include "IoContextImpl.hpp"

namespace EventLoop
{
    std::shared_ptr<ListenerComponent>
    ListenerComponent::New(const IOContextImplPtr &ContextEvent, ListenerComponent::CallbackDelegate &&Callback,
                           void *Self, uint32_t Flag,
                           int backlog, socket_t listenSocket)
    {
        return std::shared_ptr<ListenerComponent>(
                new ListenerComponent(ContextEvent, std::move(Callback), Self, Flag, backlog, listenSocket));
    }

    ListenerComponent::ListenerComponent(const IOContextImplPtr &ContextEvent,
                                         ListenerComponent::CallbackDelegate &&Callback, void *Self,
                                         uint32_t Flag, int backlog, socket_t listenSocket)
            : IEventBaseComponent(ContextEvent),
              _ListenSocket(listenSocket),
              _ListenCallbackDelegate(Callback),
              _Self(Self),
              _Backlog(backlog)
    {

    }
}