//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "ListenerComponent.hpp"
#include "IoContextImpl.hpp"

namespace EventLoop
{
    ListenerComponent::ListenerComponent(const RawIOContextImplPtr &ContextEvent,
                                         ListenerComponent::CallbackDelegate &&Callback, void *Self,
                                         uint32_t Flag, int backlog, socket_t listenSocket)
            : IEventBaseComponent(ContextEvent),
              _ListenSocket(listenSocket),
              _ListenCallbackDelegate(Callback),
              _Self(Self),
              _Backlog(backlog)
    {

    }

    ListenerComponent::~ListenerComponent()
    {

    }
}