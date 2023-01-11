//
// Created by junmkim on 2020-11-12.
//
#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeIOContextImpl.hpp"
#include "LikeUnixListenerComponent.h"
#include "IoContextThreadPool.hpp"
#include "NetworkHelpers.hpp"

#ifdef POSIX_PLATFORM

namespace EventLoop
{
	LikeUnixListenerComponent::LikeUnixListenerComponent(RawIOContextImplPtr const& ContextEvent,
		ListenerComponent::CallbackDelegate&& Callback, void* Self,
		descriptor_t listenSocket)
		: ListenerComponent(ContextEvent, std::move(Callback), Self, listenSocket)
	{
		::MuteNet::SocketDescriptorHelper::SetSocketNonblock(_Listener->GetDescriptor());
		_Listener->SetCallback(ReadCallback, WriteCallback, ExceptCallback, this);

		int domain = 0;
		socklen_t size = sizeof(domain);
		if (SOCKET_ERROR == getsockopt(_Listener->GetDescriptor(), SOL_SOCKET, SO_DOMAIN, &domain, &size))
		{
			int dwErrVal = errno;
			std::error_code ec(dwErrVal, std::system_category());
			throw std::system_error(ec, "LikeUnixListenerComponent : Can't Get Socket Option Address Family");
		}

		switch (domain)
		{
		case AF_INET6:
			client_addr = (struct sockaddr*)new sockaddr_in6();
			clientAddresLength = sizeof(struct sockaddr_in6);
			break;
		case AF_INET:
			client_addr = (struct sockaddr*)new sockaddr_in();
			clientAddresLength = sizeof(struct sockaddr_in);
			break;
		default:
			throw std::logic_error("LikeUnixListenerComponent : Isn't compatible Address Family");
			break;
		}

		reinterpret_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->Enable(_Listener);
	}

	LikeUnixListenerComponent::~LikeUnixListenerComponent()
	{
		delete client_addr;
	}

	void LikeUnixListenerComponent::ReadCallback(DescriptorPtr Ptr, void* Self)
	{
		assert(Self != nullptr);
		auto ListenComponentPtr = reinterpret_cast<LikeUnixListenerComponent*>(Self);
		assert(ListenComponentPtr->_Listener != Ptr);

		ListenComponentPtr->Accept();
	}

	void LikeUnixListenerComponent::WriteCallback(DescriptorPtr Ptr, void* Self)
	{
		throw std::logic_error("LikeUnixListenerComponent : WriteCallback Called");
	}

	void LikeUnixListenerComponent::ExceptCallback(DescriptorPtr Ptr, uint16_t What, void* Self)
	{
		throw std::logic_error("LikeUnixListenerComponent : Except Called");
	}

	void LikeUnixListenerComponent::Accept()
	{
		if (_Listener->GetDescriptor() == INVALID_SOCKET)
		{
			return;
		}

		descriptor_t client = accept(_Listener->GetDescriptor(),
			(struct sockaddr*)&client_addr, &clientAddresLength);

		if (client == SOCKET_ERROR)
		{
			Accept();
			return;
		}

		if (::MuteNet::SocketDescriptorHelper::SetSocketNonblock(client) == SOCKET_ERROR)
		{
			Accept();
			return;
		}

		auto& ThreadPool = reinterpret_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->GetThreadPool();
		const auto Dispatch = [&, client_addr = client_addr, client_addr_len = clientAddresLength]()
		{
			_ListenCallbackDelegate((ListenerComponent*)this,
				reinterpret_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->CreateDescriptor(client),
				(sockaddr*)&client_addr,
				client_addr_len,
				_Self);
		};
		ThreadPool->EnqueuePermanentJob(Dispatch);
	}

	void LikeUnixListenerComponent::Disable()
	{
		reinterpret_cast<IUnixLikeIOContextImpl*>(_ContextPtr)->Disable(_Listener);
	}

	void LikeUnixListenerComponent::Free()
	{
		Disable();

		delete this;
	}
}

#endif
