//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "ListenerComponent.hpp"
#include "IoContextImpl.hpp"

namespace EventLoop
{
    IOContextImpl::IOContextImpl(IOContextEvent &Event,
                                 const uint32_t NumOfWorkerThread,
                                 const uint32_t Timeout)
            : _Event(Event)
    {
        _ThreadPool = std::make_shared<IOContextThreadPool>(NumOfWorkerThread, Timeout);
    }

    ListenerPtr IOContextImpl::CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag, int backlog,
                       socket_t listenSocket) {
        return std::shared_ptr<ListenerComponent>(
                new ListenerComponent(this, std::move(Callback), Self, Flag, backlog, listenSocket));
    }
}
