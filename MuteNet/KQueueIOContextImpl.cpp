//
// Created by JunMin Kim on 20. 11. 15..
//

#if defined(IOCONTEXT_KQUEUE)

#include "Common.h"
#include "TypeDefine.hpp"
#include "KQueueIOContextImpl.hpp"
#include <sys/event.h>

const int kReadEvent = 1;
const int kWriteEvent = 2;

namespace EventLoop
{
    KQueueIOContextImpl::KQueueIOContextImpl(IOContextEvent &Event, const uint32_t NumOfWorkerThread,
                                             const uint32_t Timeout) :
            IUnixLikeIOContextImpl(Event, NumOfWorkerThread, Timeout), _kqueue(kqueue())
    {
    }

    void KQueueIOContextImpl::DoWork()
    {
        const auto GetDescriptorPtr = [&](const descriptor_t descriptor) -> DescriptorPtr
        {
            SharedScopedLockObject<DescriptorPerSocketPtrMapWithLock> Lock(_SocketMap);

            const auto Iter = _SocketMap.find(descriptor);

            if (_SocketMap.end() == Iter)
            {
                return nullptr;
            }

            return Iter->second;
        };


        const size_t MaxEvents = 1000;
        struct kevent ActiveEvents[MaxEvents];

        struct timespec timeout;
        timeout.tv_sec = _Timeout;
        timeout.tv_nsec = (_Timeout % 1000) * 1000 * 1000;

        const auto &ThreadPool = GetThreadPool();

        while (!IsStop())
        {
            int ActiveEventCount = kevent(_kqueue, nullptr, 0, ActiveEvents, MaxEvents, &timeout);

            for (int counter = 0; counter < ActiveEventCount; ++counter)
            {
                descriptor_t descriptor = static_cast<descriptor_t>(reinterpret_cast<intptr_t>(ActiveEvents[counter].udata));
                int event = ActiveEvents[counter].filter;

                auto DescriptorPtr = GetDescriptorPtr(descriptor);

                if (nullptr == DescriptorPtr)
                {
                    UpdateEvent(descriptor, kReadEvent & kWriteEvent, true);
                    continue;
                }

                const auto Dispatch = [&]()
                {
                    if (event == EVFILT_READ)
                    {
                        if (!DescriptorPtr->_Read())
                        {
                            Disable(DescriptorPtr);
                        }
                    }

                    if (event == EVFILT_WRITE)
                    {
                        if (!DescriptorPtr->_Write())
                        {
                            Disable(DescriptorPtr);
                        }
                    }

                    if (event == EVFILT_EXCEPT)
                    {
                        if (DescriptorPtr->_ExceptCallback == nullptr)
                        {

                        }
                        else
                        {
                            DescriptorPtr->_ExceptCallback(DescriptorPtr, errno, DescriptorPtr->_Key);
                        }
                    }
                };
                ThreadPool->EnqueueJob(Dispatch);
            }
        }
    }

    ListenerPtr KQueueIOContextImpl::CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self,
                                                    descriptor_t listenSocket)
    {
        return IUnixLikeIOContextImpl::CreateListener(std::move(Callback), Self, listenSocket);
    }

    DescriptorPtr KQueueIOContextImpl::CreateDescriptor(descriptor_t descriptor)
    {
        return IUnixLikeIOContextImpl::CreateDescriptor(descriptor);
    }

    bool KQueueIOContextImpl::Enable(const DescriptorPtr descriptor)
    {
        {
            std::pair<descriptor_t, DescriptorPtr> pair{descriptor->GetDescriptor(), descriptor};

            UniqueScopedLockObject<DescriptorPerSocketPtrMapWithLock> Lock(_SocketMap);
            _SocketMap.insert(pair);
        }

        return UpdateEvent(descriptor->GetDescriptor(), kReadEvent & kWriteEvent, false);
    }

    void KQueueIOContextImpl::Disable(const DescriptorPtr descriptor)
    {
        UpdateEvent(descriptor->GetDescriptor(), kReadEvent & kWriteEvent, true);

        {
            UniqueScopedLockObject<DescriptorPerSocketPtrMapWithLock> Lock(_SocketMap);

            const auto Iter = _SocketMap.find(descriptor->GetDescriptor());
            if (_SocketMap.end() == Iter)
            {
                return;
            }

            _SocketMap.erase(Iter);
        }
    }

    bool KQueueIOContextImpl::UpdateEvent(descriptor_t descriptor, int events, bool modify)
    {
        struct kevent event[4];
        int n = 0;

        if (events & kReadEvent)
        {
            EV_SET(&event[n++], descriptor, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, (void *) descriptor);
        }
        else if (modify)
        {
            EV_SET(&event[n++], descriptor, EVFILT_READ, EV_DELETE, 0, 0, (void *) descriptor);
        }

        if (events & kWriteEvent)
        {
            EV_SET(&event[n++], descriptor, EVFILT_WRITE, EV_ADD | EV_ENABLE, 0, 0, (void *) descriptor);
        }
        else if (modify)
        {
            EV_SET(&event[n++], descriptor, EVFILT_WRITE, EV_DELETE, 0, 0, (void *) descriptor);
        }

        int result = kevent(_kqueue, event, n, nullptr, 0, nullptr);

        if (result == SOCKET_ERROR)
        {
            return false;
        }

        return true;

    }

}
#endif