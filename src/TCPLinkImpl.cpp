//
// Created by Mute on 2020-11-18.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "NetworkHelpers.hpp"
#include "TCPLinkImpl.hpp"

#include <utility>
#include "IoContextEvent.hpp"
#include "Descriptor.h"

namespace MuteNet
{
    class TCPLinkImplHelper
    {
    public:
        static void UpdateLocalAddress(TCPLinkImpl *TcpLink)
        {
            sockaddr_storage sa{};
            auto salen = static_cast<socklen_t>(sizeof(sa));
            getsockname(TcpLink->_DescriptorPtr->GetDescriptor(), reinterpret_cast<sockaddr *>(&sa), &salen);
            UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, TcpLink->_LocalIP, TcpLink->_LocalPort);
        }

        static void UpdateRemoteAddress(TCPLinkImpl *TcpLink)
        {
            sockaddr_storage sa{};
            auto salen = static_cast<socklen_t>(sizeof(sa));
            getpeername(TcpLink->_DescriptorPtr->GetDescriptor(), reinterpret_cast<sockaddr *>(&sa), &salen);
            UpdateAddress(reinterpret_cast<const sockaddr *>(&sa), salen, TcpLink->_RemoteIP, TcpLink->_RemotePort);
        }

        static void UpdateAddress(const sockaddr *Address, socklen_t AddrLen, std::string &IP, uint16_t &Port)
        {
            char temp[128];
            switch (Address->sa_family)
            {
                case AF_INET:  // IPv4:
                {
                    const auto *sin = reinterpret_cast<const sockaddr_in *>(Address);
                    SocketDescriptorHelper::InetNtop(AF_INET, &(sin->sin_addr), temp, sizeof(IP));
                    Port = ntohs(sin->sin_port);
                    break;
                }
                case AF_INET6:  // IPv6
                {
                    const auto *sin = reinterpret_cast<const sockaddr_in6 *>(Address);
                    SocketDescriptorHelper::InetNtop(AF_INET6, &(sin->sin6_addr), temp, sizeof(IP));
                    Port = ntohs(sin->sin6_port);
                    break;
                }

                default:
                {
                    // "%s: Unknown socket address family: %d", __FUNCTION__, Address->sa_family
                    // (!"Unknown socket address family");
                    break;
                }
            }
            IP.assign(temp);
        }

    };

    bool TCPLinkImpl::Send(const void *Data, size_t Length)
    {
        _DescriptorPtr->Write(const_cast<void *>(Data), Length);
        return true;
    }

    std::string TCPLinkImpl::GetLocalIP() const
    {
        return _LocalIP;
    }

    uint16_t TCPLinkImpl::GetLocalPort() const
    {
        return _LocalPort;
    }

    std::string TCPLinkImpl::GetRemoteIP() const
    {
        return _RemoteIP;
    }

    uint16_t TCPLinkImpl::GetRemotePort() const
    {
        return _RemotePort;
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

    TCPLinkImpl::TCPLinkImpl(EventLoop::IOContextEvent &EventBase, const TCPLink::CallbacksPtr &LinkCallback) :
            TCPLink(LinkCallback), _EventBase(EventBase)
    {

    }

    TCPLinkImpl::TCPLinkImpl(EventLoop::IOContextEvent &EventBase, DescriptorPtr socket,
                             const TCPLink::CallbacksPtr &LinkCallback,
                             const ServerHandleImplPtr &ServerHandlePtr, const sockaddr *Addr, size_t socketLen)
            :
            TCPLink(LinkCallback), _EventBase(EventBase)
    {
        TCPLinkImplHelper::UpdateLocalAddress(this);
        TCPLinkImplHelper::UpdateAddress(Addr, socketLen, _RemoteIP, _RemotePort);
    }

    TCPLinkImpl::~TCPLinkImpl()
    = default;

    TCPLinkImplPtr TCPLinkImpl::Connect(EventLoop::IOContextEvent &EventBase, std::string &Host, uint16_t Port,
                                        const TCPLink::CallbacksPtr &LinkCallbacks,
                                        NetworkHelpers::ConnectCallbacksPtr ConnectCallbacks)
    {
        TCPLinkImplPtr link{new TCPLinkImpl(EventBase, LinkCallbacks)};
        link->_ConnectCallbacks = std::move(ConnectCallbacks);

        class DomainCallbacks : public NetworkHelpers::ResolveDomainNameCallbacks
        {
            TCPLinkImplPtr _link;
            uint16_t _port;
            bool _isConnecting;
        public:
            DomainCallbacks(TCPLinkImplPtr Link, uint16_t Port)
                    :
                    _link(std::move(Link)), _port(Port), _isConnecting(false)
            {
            }

            bool OnNameResolvedIPv4(const std::string &Name, const sockaddr_in *Ip) override
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
                        int ErrCode = SocketDescriptorHelper::Connect(_link->_DescriptorPtr, const_cast<sockaddr *>(Ip),
                                                                      size);
                        if (ErrCode == 0)
                        {
                            _isConnecting = true;
                        }
                        else
                        {
                            _link->GetCallbacks()->OnError(ErrCode, SocketDescriptorHelper::ErrorString(ErrCode));
                        }
                    }
                }

            void OnError(int ErrorCode, const std::string &ErrorMsg) override
            {
                _link->GetCallbacks()->OnError(ErrorCode, ErrorMsg);

            }

            void OnNameResolved(const std::string &Name, const std::string &IP) override
            {
            }

            void OnFinished() override
            {
            }
        };

        NetworkHelpers::HostnameToIP(Host, std::make_shared<DomainCallbacks>(link, Port));

        return link;
    }

    void TCPLinkImpl::Enable(const TCPLinkImplPtr &Self)
    {
        _EventBase.Enable(_DescriptorPtr);

        _DescriptorPtr->SetCallback(RecvCallback, SendCallback, ExceptCallback, this);
    }


    void TCPLinkImpl::RecvCallback(DescriptorPtr Ptr, void *Self)
    {
        assert(Self != nullptr);
        auto *SelfPtr = static_cast<TCPLinkImpl *>(Self);
        assert(SelfPtr->_DescriptorPtr != Ptr);
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
        assert(Self != nullptr);
        auto *SelfPtr = static_cast<TCPLinkImpl *>(Self);


    }

}