//
// Created by junmkim on 2020-11-16.
//

#ifndef MUTENET_EPOLLCONTEXTIMPL_HPP
#define MUTENET_EPOLLCONTEXTIMPL_HPP

#if defined(IOCONTEXT_EPOLL)

#include "UnixLikeIOContextImpl.hpp"
#include "TypeDefine.hpp"
#include "LikeUnixListenerComponent.h"

#include <Runnable.hpp>

namespace EventLoop
{
    class EpollContextImpl : public IUnixLikeIOContextImpl, public Util::Runnable
    {
        descriptor_t        _EpollHandle;

    public:
        EpollContextImpl(IOContextEvent &Event,
                         const uint32_t NumOfWorkerThread, const uint32_t Timeout);
        virtual ~EpollContextImpl();

        virtual ListenerPtr
        CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, descriptor_t listenSocket) override;

        virtual DescriptorPtr CreateDescriptor(socket_t Socket) override;

        virtual bool Enable(const DescriptorPtr descriptor) override;

        virtual void Disable(const DescriptorPtr descriptor) override;

    protected:
        virtual void DoWork() override;

    };
}


#endif

#endif //MUTENET_EPOLLCONTEXTIMPL_HPP
