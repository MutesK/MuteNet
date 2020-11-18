//
// Created by Mute on 2020-11-18.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "NetworkHelpers.hpp"
#include "TCPLinkImpl.hpp"
#include "IoContextEvent.hpp"
#include "Descriptor.h"

namespace MuteNet
{
    bool TCPLinkImpl::Send(const void *Data, size_t Length)
    {
        _DescriptorPtr->Write(const_cast<void *>(Data), Length);
        return true;
    }

    std::string TCPLinkImpl::GetLocalIP(void) const
    {
        return std::string();
    }

    uint16_t TCPLinkImpl::GetLocalPort() const
    {
        return 0;
    }

    std::string TCPLinkImpl::GetRemoteIP() const
    {
        return std::string();
    }

    uint16_t TCPLinkImpl::GetRemotePort() const
    {
        return 0;
    }

    void TCPLinkImpl::Shutdown()
    {
#ifdef _WIN32
        _DescriptorPtr->Shutdown(SD_SEND);
#else
        _DescriptorPtr->Shutdown(SHUT_WR);
#endif
        _EventBase.Disable(_DescriptorPtr);
    }

    void TCPLinkImpl::Close()
    {
        _EventBase.Disable(_DescriptorPtr);
    }

    TCPLinkImpl::TCPLinkImpl(EventLoop::IOContextEvent& EventBase, const TCPLink::CallbacksPtr LinkCallback) :
            TCPLink(LinkCallback), _EventBase(EventBase)
    {

    }

    TCPLinkImpl::TCPLinkImpl(EventLoop::IOContextEvent& EventBase, descriptor_t socket, const TCPLink::CallbacksPtr LinkCallback,
                            const ServerHandleImplPtr ServerHandlePtr, const sockaddr *Addr, size_t socketLen)
                             : TCPLink(LinkCallback), _EventBase(EventBase)
    {

    }

    TCPLinkImpl::~TCPLinkImpl()
    {

    }

    TCPLinkImplPtr TCPLinkImpl::Connect(EventLoop::IOContextEvent& EventBase, std::string &Host, uint16_t Port, TCPLink::CallbacksPtr LinkCallbacks,
                                     NetworkHelpers::ConnectCallbacksPtr ConnectCallbacks)
    {
        TCPLinkImplPtr link { new TCPLinkImpl(EventBase, LinkCallbacks) };
        link->_ConnectCallbacks = ConnectCallbacks;

        class DomainCallbacks  : public NetworkHelpers::ResolveDomainNameCallbacks
        {
                TCPLinkImplPtr _link;
                uint16_t	_port;
                bool		_isConnecting;
        public:
                DomainCallbacks(TCPLinkImplPtr Link, uint16_t Port)
                :_link(std::move(Link)), _port(Port), _isConnecting(false)
                {
                }

                bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) override
                {
                    sockaddr_in addr = *Ip;
                    addr.sin_port = htons(_port);

                    Connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
                    return false;
                }

                bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) override
                {
                    sockaddr_in6 addr = *Ip;
                    addr.sin6_port = htons(_port);

                    Connect(reinterpret_cast<const sockaddr*>(&addr), sizeof(addr));
                    return false;
                }

                void Connect(const sockaddr* Ip, int size)
                {
                    if (!_isConnecting)
                    {
                        int ErrCode = SocketUtil::Connect(_link, Ip, size);
                        if (ErrCode == 0)
                        {
                            _isConnecting = true;
                        }
                        else
                        {
                            _link->GetCallbacks()->OnError(ErrCode, SocketUtil::ErrorString(ErrCode));
                        }
                    }
                }

                void OnError(int ErrorCode, const std::string& ErrorMsg) override
                {
                    _link->GetCallbacks()->OnError(ErrorCode, ErrorMsg);
                    NetworkManager::Get().RemoveLink(_link.get());
                }

                void OnNameResolved(const std::string& Name, const std::string& IP) override
                {
                }

                void OnFinished(void)
                {
                }
        };

        NetworkHelpers::HostnameToIP(Host, std::make_shared<DomainCallbacks>(link, Port));

        return link;
    }

    void TCPLinkImpl::Enable(TCPLinkImplPtr Self)
    {
        _EventBase.Enable(_DescriptorPtr);

        _DescriptorPtr->SetCallback(RecvCallback, SendCallback, ExceptCallback, this);

        _DescriptorPtr->Enable();
    }


    void TCPLinkImpl::RecvCallback(DescriptorPtr Ptr, void *Self)
    {
        assert(Self != nullptr);
        TCPLinkImpl * SelfPtr = static_cast<TCPLinkImpl *>(Self);
        assert(SelfPtr->_DescriptorPtr == Ptr);
        assert(SelfPtr->_Callback != nullptr);

        Util::InputMemoryStream Buffer = Ptr->GetReadBuffer();
    }

    void TCPLinkImpl::SendCallback(DescriptorPtr Ptr, void *Self)
    {
        assert(Self != nullptr);
        auto SelfPtr = static_cast<TCPLinkImpl *>(Self);
        assert(SelfPtr->_Callback != nullptr);

    }

    void TCPLinkImpl::ExceptCallback(DescriptorPtr Ptr, uint16_t What, void *Self)
    {

    }

}