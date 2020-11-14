//
// Created by junmkim on 2020-11-13.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "SelectIOContext.hpp"
#include "SocketDescriptor.h"

namespace EventLoop
{

    SelectIOContext::SelectIOContext ( IOContextEvent &Event, const uint32_t NumOfWorkerThread, const uint32_t Timeout )
            : IOContextImpl ( Event, NumOfWorkerThread, Timeout )
    {

    }

    void SelectIOContext::DoWork ( )
    {
        socket_t MaxDescriptorNumber = 0;
        ContextContainer Container;

        struct    timeval tv;
        tv.tv_sec = _Timeout;

        while(!IsStop())
        {
            Container = _Container;

            FD_ZERO(&_ReadSet);
            FD_ZERO(&_WriteSet);

            FDSet(Container);

            int ndfs = Container._RegisteredSockets.size();
            int result = select(ndfs, &_ReadSet, &_WriteSet, nullptr, &tv);

            if(result == -1 || result == 0)
            {
                continue;
            }

            FDIsSetAndCallback(Container);
        }
    }

    SelectIOContext::ContextContainer
    SelectIOContext::ContextContainer::operator= ( SelectIOContext::ContextContainer &Container )
    {
        std::lock_guard<std::mutex> lock(Container._SwapMutex);

        _RegisteredSockets.swap(Container._RegisteredSockets);
    }

    void SelectIOContext::ContextContainer::EnqueueSocket ( const SocketPtr &Ptr )
    {
        std::lock_guard<std::mutex> lock(_SwapMutex);

        _RegisteredSockets.insert(Ptr);
    }

    void SelectIOContext::FDSet ( ContextContainer& Container )
    {
        std::for_each(Container._RegisteredSockets.begin(), Container._RegisteredSockets.end(),
                      [&](SocketPtr& Ptr)
        {
            FD_SET(Ptr->_socket, &_ReadSet);
            FD_SET(Ptr->_socket, &_WriteSet);
        });
    }

    void SelectIOContext::FDIsSetAndCallback ( SelectIOContext::ContextContainer &Container )
    {
        std::for_each(Container._RegisteredSockets.begin(), Container._RegisteredSockets.end(),
                      [&](SocketPtr& Ptr)
                      {
                         const auto& ThreadPool = GetThreadPool();
                         const static auto DispatchSignal = [&]()
                         {
                             if(FD_ISSET(Ptr->_socket, &_ReadSet))
                             {
                                 Ptr->_ReadCallback(Ptr.get(), Ptr->_Key);
                             }
                             if(FD_ISSET(Ptr->_socket, &_WriteSet))
                             {
                                 Ptr->_WriteCallback(Ptr.get(), Ptr->_Key);
                             }
                         };

                          ThreadPool->EnqueueJob(DispatchSignal);
                      });
    }

    SocketPtr SelectIOContext::CreateSocket ( socket_t Socket )
    {
        const auto& Ptr = IOContextImpl::CreateSocket ( Socket );

        _Container.EnqueueSocket(Ptr);
        return Ptr;
    }
	
	ListenerPtr
	SelectIOContext::CreateListener ( ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag,
	                                  int backlog, socket_t listenSocket )
	{
		return EventLoop::ListenerPtr ( );
	}
}