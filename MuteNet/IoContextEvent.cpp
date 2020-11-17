//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IoContextEvent.hpp"

#include "IocpContextImpl.hpp"
#include "KQueueIOContextImpl.hpp"
#include "SelectIOContext.hpp"
#include "EpollContextImpl.hpp"

namespace EventLoop
{
    IOContextImplPtr CreateIOContext(IOContextEvent &Event, const uint32_t NumOfWorkerThread, const uint32_t Timeout)
    {
#if defined(IOCONTEXT_KQUEUE)
        return std::make_shared<KQueueIOContextImpl>(Event, NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_IOCP)
        return std::make_shared<IocpContextImpl>(Event, NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_SELECT)
        return std::make_shared<KQueueIOContextImpl>(Event, NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_EPOLL)
        return std::make_shared<KQueueIOContextImpl>(Event, NumOfWorkerThread, Timeout);
#endif
        return nullptr;
    }

    IOContextEvent::IOContextEvent(const uint32_t NumOfWorkerThread, const uint32_t Timeout)
    {

    }

    IOContextEvent::~IOContextEvent() {

    }
}

