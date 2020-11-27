//
// Created by JunMin Kim on 20. 11. 15..
//

#ifndef MUTENET_KQUEUEIOCONTEXTIMPL_HPP
#define MUTENET_KQUEUEIOCONTEXTIMPL_HPP

#if defined(IOCONTEXT_KQUEUE)

#include "UnixLikeIOContextImpl.hpp"
#include "Runnable.hpp"

namespace EventLoop
{
    class KQueueIOContextImpl : public IUnixLikeIOContextImpl, public Util::Runnable
    {
        descriptor_t _kqueue;
    public:
        KQueueIOContextImpl(IOContextEvent &Event,
                            const uint32_t NumOfWorkerThread, const uint32_t Timeout);

        virtual ListenerPtr
        CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, descriptor_t listenSocket) override;

        virtual DescriptorPtr CreateDescriptor(descriptor_t descriptor) override;

        virtual bool Enable(const DescriptorPtr descriptor) override;

        virtual void Disable(const DescriptorPtr descriptor) override;

    protected:
        virtual void DoWork() override;

        bool UpdateEvent(descriptor_t descriptor, int events, bool modify);
    };
}

#endif
#endif //MUTENET_KQUEUEIOCONTEXTIMPL_HPP
