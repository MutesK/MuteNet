//
// Created by JunMin Kim on 20. 11. 15..
//

#if defined(IOCONTEXT_KQUEUE)

#include "Common.h"
#include "TypeDefine.hpp"
#include "KQueueIOContextImpl.hpp"
#include <sys/event.h>

const int kRead = 1;
const int kWrite = 2;

namespace EventLoop
{
    KQueueIOContextImpl::KQueueIOContextImpl(IOContextEvent &Event, const uint32_t NumOfWorkerThread,
                                             const uint32_t Timeout) :
            IUnixLikeIOContextImpl(Event, NumOfWorkerThread, Timeout), _kQueue(kqueue())
    {
    }

    void KQueueIOContextImpl::DoWork()
    {
        ContextContainer Container;

        while(!IsStop())
        {
            Container = _Container;



        }
    }

    void KQueueIOContextImpl::RegisterHandle(int fd, uint8_t filter, uint16_t flag)
    {
        EventHandle(fd, filter, EV_ADD);
    }

    void KQueueIOContextImpl::RemoveEvent(int fd, uint8_t filter)
    {
        EventHandle(fd, filter, EV_DELETE);
    }

    void KQueueIOContextImpl::EnableEvent(int fd, uint8_t filter)
    {
        EventHandle(fd, filter, EV_ENABLE);
    }

    void KQueueIOContextImpl::DisableEvent(int fd, uint8_t filter)
    {
        EventHandle(fd, filter, EV_DISABLE);
    }

    void KQueueIOContextImpl::EventHandle(int fd, uint8_t filter, uint16_t flag)
    {
        struct kevent changelist[2];
        int n = 0;

        if(filter & kRead)
            EV_SET(&changelist[n++], fd, EVFILT_READ, flag, 0, 0, NULL);

        if(filter & kWrite)
            EV_SET(&changelist[n++], fd, EVFILT_WRITE, flag, 0, 0, NULL);

        if(kevent(_kQueue, changelist, n, NULL, 0, NULL) == -1)
        {

        }

    }
}
#endif