//
// Created by JunMin Kim on 2020/11/11.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "IoContextEvent.hpp"
#include "Descriptor.h"

#include "IocpContextImpl.hpp"
#include "KQueueIOContextImpl.hpp"
#include "SelectIOContext.hpp"
#include "EpollContextImpl.hpp"

namespace EventLoop
{
    IOContextImplPtr CreateIOContext(const uint32_t NumOfWorkerThread, const uint32_t Timeout)
    {
#if defined(IOCONTEXT_KQUEUE)
        return std::make_shared<KQueueIOContextImpl>( NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_IOCP)
        return std::make_shared<IocpContextImpl>(NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_SELECT)
        return std::make_shared<KQueueIOContextImpl>( NumOfWorkerThread, Timeout);
#elif defined(IOCONTEXT_EPOLL)
        return std::make_shared<EpollContextImpl>( NumOfWorkerThread, Timeout);
#endif
        std::runtime_error("Can't find IOContext Type!");
    }

    IOContextEvent::IOContextEvent(const uint32_t NumOfWorkerThread, const uint32_t Timeout)
        :_ContextImpl(CreateIOContext(NumOfWorkerThread, Timeout))
    {

    }

    IOContextEvent::~IOContextEvent()
    {

    }

    ListenerPtr IOContextEvent::CreateListener(ListenerComponent::CallbackDelegate Callback, void *Self,
                                               descriptor_t listenSocket)
    {
        return _ContextImpl->CreateListener(Callback, Self, listenSocket);
    }

    DescriptorPtr IOContextEvent::CreateDescriptor(descriptor_t descriptor)
    {
        return _ContextImpl->CreateDescriptor(descriptor);
    }

    bool IOContextEvent::Enable(DescriptorPtr descriptor)
    {
        return _ContextImpl->Enable(descriptor);
    }

    void IOContextEvent::Disable(DescriptorPtr descriptor)
    {
        _ContextImpl->Disable(descriptor);
    }
}

