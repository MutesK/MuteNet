#include "pch.h"
#include "Acceptor.h"
#include "ServiceListener.h"
#include "SocketUtil.h"
#include "ASyncAcceptRequest.h"

using namespace Util;

namespace MuteNet
{
    Util::TL::ObjectPool<ASyncAcceptRequest, true, true> ASyncAcceptRequest::OverlappedPool(400);

    AcceptorPtr Acceptor::Listen(ServiceListener *Port, ListenerCallback Callback, ListenerErrorCallback ErrorCallback,
                                 SOCKADDR_IN *Ip, void *key, int Backlog)
    {
        assert(Callback != nullptr);
        assert(Port != nullptr);

        const auto FuncExt = Port->GetExtension();

        assert(FuncExt != nullptr);
        assert(FuncExt->_AcceptEx != nullptr);
        assert(FuncExt->_GetAcceptExSockaddrs != nullptr);

        AcceptorPtr Ptr{new Acceptor(const_cast<ServiceListener *>(Port), Callback, ErrorCallback, Ip, key, Backlog)};

        Ptr->_Self = Ptr;
        Ptr->InitializeListenSocket();
        Ptr->Start();

        return Ptr;
    }

    Acceptor::Acceptor(ServiceListener *Port, ListenerCallback &Callback, ListenerErrorCallback &ErrorCallback,
                       SOCKADDR_IN *Ip, void *key, int Backlog)
            : _Port(Port), _Callback(std::move(Callback)), _Address(Ip), _Listen(INVALID_SOCKET),
              _Backlog(Backlog), _ErrorCallback(ErrorCallback), _Key(key)
    {
        _StopTrigger = false;
    }

    void Acceptor::InitializeListenSocket()
    {
        int error;

        _Listen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (_Listen == INVALID_SOCKET)
        {
            error = WSAGetLastError();

            _ErrorCallback(_Listen, error, SocketUtil::ErrorString(error));
            return;
        }

        _Port->RegisterHandle(reinterpret_cast<void *>(_Listen), reinterpret_cast<void *>(1));

        error = std::invoke(bind, _Listen,
                            reinterpret_cast<sockaddr *>(_Address), sizeof(SOCKADDR_IN));
        if (error != ERROR_SUCCESS)
        {
            _ErrorCallback(_Listen, error, SocketUtil::ErrorString(error));
            return;
        }

        SocketUtil::NonBlockSocket(_Listen);

        error = std::invoke(::listen, _Listen, _Backlog);
        if (error != ERROR_SUCCESS)
        {
            _ErrorCallback(_Listen, error, SocketUtil::ErrorString(error));
            return;
        }
    }

    void Acceptor::StartAccept() const
    {
        if (_StopTrigger)
        {
            return;
        }

        const auto FuncExt = _Port->GetExtension();

        auto AcceptTask = ASyncAcceptRequest::GetAcceptRequest(_Self, FuncExt->_AcceptEx,
                                                               FuncExt->_GetAcceptExSockaddrs);

        AcceptTask->Overlapped.SelfPtr = AcceptTask;

        if (!AcceptTask->Process())
        {
            ASyncAcceptRequest::FreeAcceptRequest(AcceptTask);
        }
    }

    void Acceptor::Start()
    {
        _StopTrigger = false;

        for (uint32_t i = 0; i < std::thread::hardware_concurrency(); ++i)
            StartAccept();

    }

    void Acceptor::Stop()
    {
        _StopTrigger = true;
    }

}
