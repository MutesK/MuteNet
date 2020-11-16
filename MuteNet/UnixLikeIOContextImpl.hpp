//
// Created by JunMin Kim on 20. 11. 15..
//

#ifndef MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
#define MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "IoContextImpl.hpp"
#include <Runnable.hpp>

namespace EventLoop {
    class IUnixLikeIOContextImpl : public IOContextImpl
            {
    public:
        IUnixLikeIOContextImpl(IOContextEvent &Event,
                               const uint32_t NumOfWorkerThread, const uint32_t Timeout);

        virtual ListenerPtr
        CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag, int backlog,
                       socket_t listenSocket) override;

        virtual DescriptorPtr CreateSocket(socket_t Socket) override;

        virtual bool Enable(const DescriptorPtr descriptor) = 0;
        virtual void Disable(const DescriptorPtr descriptor) = 0;
    };
}

#endif
#endif //MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
