//
// Created by JunMin Kim on 20. 11. 15..
//

#ifndef MUTENET_KQUEUEIOCONTEXTIMPL_HPP
#define MUTENET_KQUEUEIOCONTEXTIMPL_HPP

#if defined(IOCONTEXT_KQUEUE)

#include "UnixLikeIOContextImpl.hpp"

namespace EventLoop
{
    class KQueueIOContextImpl : public IUnixLikeIOContextImpl
    {
        using KQueueHandle = int32_t;

        struct KQueueContainer : public IContextContainerImpl
        {
            virtual void Register(const socket_t socket) override;

            virtual void UnRegister(const socket_t socket) override;

        };

        KQueueHandle _kQueue;
    public:
        KQueueIOContextImpl ( IOContextEvent &Event,
        const uint32_t NumOfWorkerThread, const uint32_t Timeout );

    protected:
        virtual void DoWork() override;

        void RegisterHandle(int fd, uint8_t filter, uint16_t flag);
        void RemoveEvent(int fd, uint8_t filter);
        void EnableEvent(int fd, uint8_t filter);
        void DisableEvent(int fd, uint8_t filter);
        void EventHandle(int fd, uint8_t filter, uint16_t flag);
    };
}

#endif
#endif //MUTENET_KQUEUEIOCONTEXTIMPL_HPP
