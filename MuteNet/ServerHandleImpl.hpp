//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_SERVERHANDLEIMPL_HPP
#define MUTENET_SERVERHANDLEIMPL_HPP

#include "ServerHandle.hpp"

namespace EventLoop
{
    class ListenerComponent;
    class IOContextEvent;
    class IDescriptor;
}

namespace MuteNet
{
    using DescriptorPtr = EventLoop::IDescriptor*;

    class ListenerComponent;
    using ListenerPtr = std::shared_ptr<EventLoop::ListenerComponent>;

    class TCPLinkImpl;

    class ServerHandleImpl;
    using ServerHandleImplPtr = std::shared_ptr<ServerHandleImpl>;

    class ServerHandleImpl : public ServerHandle
    {
    protected:
        EventLoop::IOContextEvent&          _EventBase;
        NetworkHelpers::ListenCallbacksPtr _ListenCallbacks;
        ListenerPtr                        _Listener;
        bool                               _IsListening;
        ServerHandleImplPtr                    _Self;

        uint64_t                            _ErrorCode;
        std::string                         _ErrorMsg;
    public:
        virtual ~ServerHandleImpl();

        static ServerHandleImplPtr Listen(
                EventLoop::IOContextEvent& EventBase,
                uint16_t a_Port,
                NetworkHelpers::ListenCallbacksPtr a_ListenCallbacks
        );

        virtual void Close() override;

        virtual bool IsListening() const override;

    protected:
        ServerHandleImpl(EventLoop::IOContextEvent&  EventBase,
                         NetworkHelpers::ListenCallbacksPtr a_ListenCallbacks);

        bool Listen(uint16_t a_Port);

        static void Callback(EventLoop::ListenerComponent *pRawListener, DescriptorPtr a_Socket, sockaddr * a_Addr, int a_Len, void * a_Self);


    };
}

#endif //MUTENET_SERVERHANDLEIMPL_HPP
