//
// Created by Mute on 2020-11-18.
//

#ifndef MUTENET_TCPLINKIMPL_HPP
#define MUTENET_TCPLINKIMPL_HPP

#include "NetworkHelpers.hpp"

namespace EventLoop
{
    class IDescriptor;
    class IOContextEvent;
}

namespace MuteNet
{
    using DescriptorPtr = EventLoop::IDescriptor*;

    class TCPLinkImpl;
    using TCPLinkImplPtr = std::shared_ptr<TCPLinkImpl>;

    class ServerHandleImpl;
    typedef std::shared_ptr<ServerHandleImpl> ServerHandleImplPtr;


    class TCPLinkImpl : public TCPLink
    {
        EventLoop::IOContextEvent&          _EventBase;
        NetworkHelpers::ConnectCallbacksPtr _ConnectCallbacks;
        TCPLinkImplPtr                       _SelfPtr;
        ServerHandleImplPtr                 _ServerPtr;

        DescriptorPtr                       _DescriptorPtr;

    public:
        TCPLinkImpl(EventLoop::IOContextEvent& EventBase, const CallbacksPtr LinkCallback);
        TCPLinkImpl(EventLoop::IOContextEvent& EventBase, DescriptorPtr socket, const CallbacksPtr LinkCallback,
        const ServerHandleImplPtr ServerHandlePtr, const sockaddr* Addr, size_t socketLen);
        virtual ~TCPLinkImpl();

        static TCPLinkImplPtr Connect(EventLoop::IOContextEvent& EventBase, std::string& Host, uint16_t Port,
                                   TCPLink::CallbacksPtr LinkCallbacks,
                                   NetworkHelpers::ConnectCallbacksPtr ConnectCallbacks);

        void Enable(TCPLinkImplPtr Self);

        virtual bool Send(const void *Data, size_t Length) override;

        virtual std::string GetLocalIP(void) const override;

        virtual uint16_t GetLocalPort() const override;

        virtual std::string GetRemoteIP() const override;

        virtual uint16_t GetRemotePort() const override;

        virtual void Shutdown() override;

        virtual void Close() override;

        static void RecvCallback(DescriptorPtr Ptr, void *Self);
        static void SendCallback(DescriptorPtr Ptr, void *Self);
        static void ExceptCallback(DescriptorPtr Ptr, uint16_t What ,void *Self);
    };

}

#endif //MUTENET_TCPLINKIMPL_HPP
