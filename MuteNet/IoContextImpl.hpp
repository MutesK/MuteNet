//
// Created by JunMin Kim on 2020/11/11.
//

#ifndef MUTENET_IOCONTEXTIMPL_HPP
#define MUTENET_IOCONTEXTIMPL_HPP

#include "IoContextThreadPool.hpp"
#include <Runnable.hpp>

namespace EventLoop
{
    class IOContextEvent;
    class ListenerComponent;
    using ListenerPtr = std::shared_ptr<ListenerComponent>;

    class IOContextImpl
    {
    protected:
        IOContextEvent &_Event;
        std::shared_ptr<IOContextThreadPool> _ThreadPool;  // Using For Notify Completion
    public:
        IOContextImpl(IOContextEvent &Event,
                      const uint32_t NumOfWorkerThread, const uint32_t Timeout);

        ListenerPtr CreateListener(ListenerComponent::CallbackDelegate &&Callback,
                                                      void *Self, uint32_t Flag, int backlog, socket_t listenSocket);
    };
}


#endif //MUTENET_IOCONTEXTIMPL_HPP
