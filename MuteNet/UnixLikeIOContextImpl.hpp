//
// Created by JunMin Kim on 20. 11. 15..
//

#ifndef MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
#define MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "IoContextImpl.hpp"
#include <Runnable.hpp>

namespace EventLoop {
    class IUnixLikeIOContextImpl : public IOContextImpl, public Util::Runnable
            {
    protected:
        struct IContextContainerImpl {
            std::mutex _SwapMutex;

            ContextContainer() = default;

            virtual void Register(const socket_t Socket) = 0;
            virtual void UnRegister(const socket_t Socket) = 0;
        };
        using ContextContainerImplPtr = std::shared_ptr<IContextContainerImpl>;

        ContextContainerImplPtr _ContextContainerPtr;
    public:
        IUnixLikeIOContextImpl(IOContextEvent &Event,
                               const uint32_t NumOfWorkerThread, const uint32_t Timeout);
        ContextContainerImplPtr &GetContextContainer();

        virtual ListenerPtr
        CreateListener(ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag, int backlog,
                       socket_t listenSocket) override;

        virtual SocketPtr CreateSocket(socket_t Socket) override;

    protected:
        virtual void DoWork() = 0;
    };
}

#endif
#endif //MUTENET_UNIXLIKEIOCONTEXTIMPL_HPP
