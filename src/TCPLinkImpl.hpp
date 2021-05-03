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
		EventLoop::IOContextEvent& _EventBase;
		NetworkHelpers::ConnectCallbacksPtr _ConnectCallbacks;
		TCPLinkImplPtr                       _SelfPtr;
		ServerHandleImplPtr                 _ServerPtr;

		DescriptorPtr                       _DescriptorPtr{};

		std::string                         _LocalIP;
		uint16_t                            _LocalPort{};

		std::string                         _RemoteIP;
		uint16_t                            _RemotePort{};

		friend class TCPLinkImplHelper;
	public:
		TCPLinkImpl(EventLoop::IOContextEvent& EventBase, const CallbacksPtr& LinkCallback);
		TCPLinkImpl(EventLoop::IOContextEvent& EventBase, DescriptorPtr socket, const CallbacksPtr& LinkCallback,
			const ServerHandleImplPtr& ServerHandlePtr, const sockaddr* Addr, size_t socketLen);
		~TCPLinkImpl() override;

		static TCPLinkImplPtr Connect(EventLoop::IOContextEvent& EventBase, std::string& Host, uint16_t Port,
			const TCPLink::CallbacksPtr& LinkCallbacks,
			NetworkHelpers::ConnectCallbacksPtr ConnectCallbacks);

		void Enable(const TCPLinkImplPtr& Self);

		bool Send(const void* Data, size_t Length) override;

		std::string GetLocalIP() const override;

		uint16_t GetLocalPort() const override;

		std::string GetRemoteIP() const override;

		uint16_t GetRemotePort() const override;

		void Shutdown() override;

		void Close() override;

		static void RecvCallback(DescriptorPtr Ptr, void* Self);
		static void SendCallback(DescriptorPtr Ptr, void* Self);
		static void ExceptCallback(DescriptorPtr Ptr, uint16_t What, void* Self);
	};

}

#endif //MUTENET_TCPLINKIMPL_HPP
