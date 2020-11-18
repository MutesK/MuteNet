//
// Created by Mute on 2020-11-18.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "NetworkHelpers.hpp"
#include "ServerHandleImpl.hpp"
#include "ListenerComponent.hpp"

namespace MuteNet
{
    ServerHandleImpl::ServerHandleImpl(NetworkHelpers::ListenCallbacksPtr _ListenCallbacks)
        :_ListenCallbacks(_ListenCallbacks)
    {

    }

    void ServerHandleImpl::Close()
    {
        if(_Listener != nullptr)
        {
            _Listener->Close();
        }
    }

    bool ServerHandleImpl::IsListening() const
    {
        return false;
    }

    ServerHandleImpl::~ServerHandleImpl()
    {

    }

    ServerHandleImplPtr ServerHandleImpl::Listen(uint16_t a_Port, NetworkHelpers::ListenCallbacksPtr a_ListenCallbacks)
    {
        return MuteNet::ServerHandleImplPtr();
    }


    bool ServerHandleImpl::Listen(uint16_t a_Port)
    {
        return false;
    }

    void
    ServerHandleImpl::Callback(ListenerPtr Listener, descriptor_t a_Socket, sockaddr *a_Addr, int a_Len, void *a_Self)
    {

    }

    void ServerHandleImpl::RemoveLink(const TCPLinkImpl *a_Link)
    {

    }
}