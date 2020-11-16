//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "IoContextEvent.hpp"

namespace EventLoop
{
    IOContextImplPtr CreateIOContext(IOContextEvent &Event, const uint32_t NumOfWorkerThread, const uint32_t Timeout)
    {
#if defined(IOCONTEXT_KQUEUE)
#elif defined(IOCONTEXT_IOCP)
#elif defined(IOCONTEXT_SELECT)
#elif defined(IOCONTEXT_EPOLL)
#endif
    }

    IOContextEvent::IOContextEvent(const uint32_t NumOfWorkerThread, const uint32_t Timeout)
    {

    }

    IOContextEvent::~IOContextEvent() {

    }
}

