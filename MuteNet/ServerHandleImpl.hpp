//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_SERVERHANDLEIMPL_HPP
#define MUTENET_SERVERHANDLEIMPL_HPP

#include "ServerHandle.hpp"

namespace EventLoop
{
    class ListenerComponent;
}

namespace MuteNet
{
    class TCPLinkImpl;
    using ListenerPtr = std::shared_ptr<EventLoop::ListenerComponent>;

    class ServerHandleImpl;
    using ServerHandleImplPtr = std::shared_ptr<ServerHandleImpl>;

    class ServerHandleImpl : public ServerHandle
    {
    protected:
        NetworkHelpers::ListenCallbacksPtr _ListenCallbacks;
        ListenerPtr                        _Listener;
        bool                               _IsListening;
        ServerHandleImplPtr                    _Self;
    public:
        virtual ~ServerHandleImpl();

        static ServerHandleImplPtr Listen(
                uint16_t a_Port,
                NetworkHelpers::ListenCallbacksPtr a_ListenCallbacks
        );

        virtual void Close() override;

        virtual bool IsListening() const override;

    protected:
        ServerHandleImpl(NetworkHelpers::ListenCallbacksPtr a_ListenCallbacks);

        bool Listen(uint16_t a_Port);

        static void Callback(ListenerPtr Listener, descriptor_t a_Socket, sockaddr * a_Addr, int a_Len, void * a_Self);

        void RemoveLink(const TCPLinkImpl * a_Link);



    };
}

#endif //MUTENET_SERVERHANDLEIMPL_HPP
