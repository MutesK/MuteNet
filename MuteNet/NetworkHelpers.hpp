//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_NETWORKHELPERS_HPP
#define MUTENET_NETWORKHELPERS_HPP

#include "TCPLink.hpp"

namespace MuteNet
{
    typedef std::shared_ptr<TCPLink> LinkPtr;

    class TCPLinkImpl;
    typedef std::shared_ptr<TCPLinkImpl> TCPLinkImplPtr;
    typedef std::vector<TCPLinkImplPtr> TCPLinkImplPtrs;

    class ServerHandle;
    typedef std::shared_ptr<ServerHandle> ServerHandlePtr;

    class NetworkHelpers
    {
    public:
        class ConnectCallbacks
        {
        public:
            virtual ~ConnectCallbacks() {}

            virtual void OnConnected(TCPLink& link) = 0;
            virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
        };
        typedef std::shared_ptr<ConnectCallbacks> ConnectCallbacksPtr;

        class ListenCallbacks
        {
        public:
            virtual ~ListenCallbacks() {}

            virtual TCPLink::CallbacksPtr OnInComingConnection(const std::string& RemoteIPAddress,
                                                            uint16_t RemotePort) = 0;

            virtual void OnAccepted(TCPLink& link) = 0;

            virtual void OnError(int ErrorCode, const std::string ErrorMsg) = 0;
        };
        typedef std::shared_ptr<ListenCallbacks> ListenCallbacksPtr;

        class ResolveDomainNameCallbacks
        {
        public:
            virtual ~ResolveDomainNameCallbacks() {}

            virtual void OnNameResolved(const std::string& Name, const std::string& IP) = 0;

            virtual bool OnNameResolvedIPv4(const std::string& Name, const sockaddr_in* Ip) { return true; }

            virtual bool OnNameResolvedIPv6(const std::string& Name, const sockaddr_in6* Ip) { return true; }

            virtual void OnError(int ErrorCode, const std::string& ErrorMsg) = 0;

            virtual void OnFinished(void) = 0;
        };
        typedef std::shared_ptr<ResolveDomainNameCallbacks> ResolveDomainNameCallbacksPtr;

        static TCPLinkImplPtr Connect(const std::string& host,
                                   uint16_t port,
                                   ConnectCallbacksPtr connectCallbacks,
                                   TCPLink::CallbacksPtr LinkCallbacks);

        static ServerHandlePtr Listen(uint16_t Port,
                                      ListenCallbacksPtr listenCallbacks);

        static bool HostnameToIP(const std::string& hostName,
                                 ResolveDomainNameCallbacksPtr callbacks);

        static bool IPToHostName(const std::string& Ip,
                                 ResolveDomainNameCallbacksPtr callbacks);
    };
}

#endif //MUTENET_NETWORKHELPERS_HPP
