//
// Created by junmkim on 2020-11-16.
//

#if defined(IOCONTEXT_EPOLL)

#include "Common.h"
#include "TypeDefine.hpp"

#include <sys/epoll.h>
#include "EpollContextImpl.hpp"

/*
    // EPOLLET => Edge Trigger 사용
    // 레벨트리거와 에지 트리거를 소켓 버퍼에 대응하면, 소켓 버퍼의 레벨 즉 데이터의 존재 유무를 가지고 판단하는 것이
    // 레벨트리거 입니다.즉 읽어서 해당 처리를 하였다 하더라도 덜 읽었으면 계속 이벤트가 발생하겠지요.예를 들어
    // 1000바이트가 도착했는데 600바이트만 읽었다면 레벨 트리거에서는 계속 이벤트를 발생시킵니다.그러나
    // 에지트리거에서는 600바이트만 읽었다 하더라도 더 이상 이벤트가 발생하지 않습니다.왜냐하면 읽은 시점을 기준으로
    // 보면 더이상의 상태 변화가 없기 때문이죠..LT 또는 ET는 쉽게 옵션으로 설정 가능합니다.
    // 참고로 select / poll은 레벨트리거만 지원합니다.
 */
namespace EventLoop
{
    EpollContextImpl::EpollContextImpl(IOContextEvent &Event, const uint32_t NumOfWorkerThread, const uint32_t Timeout)
            : IUnixLikeIOContextImpl(Event, NumOfWorkerThread, Timeout), _EpollHandle(epoll_create(1024))
    {
    }

    EpollContextImpl::~EpollContextImpl()
    {
        close(_EpollHandle);
        _EpollHandle = -1;
    }

    ListenerPtr
    EpollContextImpl::CreateListener(EventLoop::ListenerComponent::CallbackDelegate &&Callback, void *Self,
                                     descriptor_t listenSocket)
    {
       return IUnixLikeIOContextImpl::CreateListener(std::move(Callback), Self,  listenSocket);
    }

    DescriptorPtr EventLoop::EpollContextImpl::CreateDescriptor(socket_t Socket)
    {
        return IUnixLikeIOContextImpl::CreateDescriptor(Socket);
    }

    bool EpollContextImpl::Enable(const DescriptorPtr descriptor)
    {
        if(nullptr == descriptor)
        {
            std::logic_error("Context Descriptor Enable Failed, Because descriptor is null");
            return false;
        }

        struct epoll_event    events;
        events.events    = EPOLLIN | EPOLLET;
        events.data.fd    = descriptor->GetDescriptor();

        if (epoll_ctl(_EpollHandle, EPOLL_CTL_ADD, descriptor->GetDescriptor(), &events) == SOCKET_ERROR)
        {
            std::error_code ec ( errno, std::system_category ( ));
            std::system_error(ec, "client epoll_ctl() error\n");
            return false;
        }

        {
            std::unique_lock<std::shared_mutex> lock(_SocketMap._mutex);

            _SocketMap[descriptor->GetDescriptor()] = descriptor;
        }

        return true;
    }

    void EpollContextImpl::Disable(const DescriptorPtr descriptor)
    {
        if(nullptr == descriptor)
        {
            std::logic_error("DescriptorPtr is nullptr");
        }

        {
            std::unique_lock<std::shared_mutex> lock(_SocketMap._mutex);

            const auto Iter = _SocketMap.find(descriptor->GetDescriptor());
            if(_SocketMap.end() == Iter)
            {
                return;
            }

            _SocketMap.erase(Iter);
        }

        epoll_ctl(_EpollHandle, EPOLL_CTL_DEL, descriptor->GetDescriptor(), nullptr);
    }

    void EpollContextImpl::DoWork()
    {
        uint64_t EventCount = 0;
        int                       MAX_EVENTS = 1024;
        struct epoll_event        epoll_events[MAX_EVENTS];

        auto& ThreadPool = GetThreadPool();
        while(!IsStop())
        {
            EventCount = epoll_wait(_EpollHandle, epoll_events, MAX_EVENTS, -1);

            if(EventCount < 0)
                continue;

            for(int counter = 0; counter < EventCount; ++counter)
            {
                descriptor_t descriptor = epoll_events[counter].data.fd;

                static const auto GetDescriptorPtr = [&]() -> DescriptorPtr
                {
                    std::shared_lock<std::shared_mutex> lock(_SocketMap._mutex);

                    const auto &Iter = _SocketMap.find(descriptor);
                    if(_SocketMap.end() == Iter)
                    {
                        return nullptr;
                    }

                    return Iter->second;
                };

                auto DescriptorPtr = GetDescriptorPtr();
                if(nullptr == DescriptorPtr)
                {
                    close(descriptor);
                    epoll_ctl(_EpollHandle, EPOLL_CTL_DEL, descriptor, nullptr);
                    continue;
                }

                const auto DispatchEvent = [&]()
                {
                    auto& event = epoll_events[counter];

                    if(event.events & EPOLLIN)
                    {
                        if(!DescriptorPtr->_Read())
                            Disable(DescriptorPtr);
                    }

                    if(event.events & EPOLLOUT)
                    {
                        if(!DescriptorPtr->_Write())
                            Disable(DescriptorPtr);
                    }

                    if(event.events & EPOLLERR)
                    {
                        if(DescriptorPtr->_ExceptCallback == nullptr)
                        {

                        }
                        else
                        {
                            DescriptorPtr->_ExceptCallback(DescriptorPtr, errno, DescriptorPtr->_Key);
                        }

                    }
                };

                ThreadPool->EnqueueJob(DispatchEvent);
            }
        }
    }

}

#endif
