//
// Created by junmkim on 2020-11-12.
//

#if defined(WIN32)

#include "Common.h"
#include "TypeDefine.hpp"
#include "Win32ListenerComponent.hpp"
#include "IocpContextImpl.hpp"
#include "IoContextThreadPool.hpp"

namespace EventLoop
{

	Win32ListenerComponent::Win32ListenerComponent(RawIOContextImplPtr const& ContextEvent,
		ListenerComponent::CallbackDelegate&& Callback, void* Self,
		descriptor_t listenSocket)
		: ListenerComponent(ContextEvent, std::move(Callback), Self, listenSocket)
	{
		reinterpret_cast<IocpContextImpl*>(_ContextPtr)->Enable(_Listener);
	}

	Win32ListenerComponent::~Win32ListenerComponent()
	{
		Disable();
	}

	void Win32ListenerComponent::AcceptRequest()
	{
		static auto& Extension = reinterpret_cast<IocpContextImpl*>(_ContextPtr)->_Extensions;
		static AcceptExPtr& _AcceptExFunctionPtr = Extension._AcceptEx;
		static DWORD Pending = 0;

		_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (!_AcceptExFunctionPtr(_Listener->GetDescriptor(), _Client, const_cast<char*>(_AddressBuffer.GetBufferPtr()), 0,
			sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Pending, &_Overlapped))
		{
			return;
		}
	}

	void Win32ListenerComponent::IOCompletion(OVERLAPPED* pRawOverlapped, uint32_t TransfferedBytes)
	{
		static auto& Extension = reinterpret_cast<IocpContextImpl*>(_ContextPtr)->_Extensions;
		static GetAcceptExSockAddrsPtr& _GetAcceptExSockAddrPtr = Extension._GetAcceptExSockaddrs;

		const auto& Pool = reinterpret_cast<IocpContextImpl*>(_ContextPtr)->GetThreadPool();

		const auto Process = [&]()
		{
			if (nullptr == pRawOverlapped)
				return;

			auto Listen = _Listener->GetDescriptor();
			if (setsockopt(_Client, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
				reinterpret_cast<char*>(&Listen), sizeof(descriptor_t)) == SOCKET_ERROR)
			{
				return;
			}

			SOCKADDR* RemoteSockAddr = nullptr, * LocalSockAddr = nullptr;
			int RemoteAddrLength = 0, LocalAddrLength = 0;

			_GetAcceptExSockAddrPtr(const_cast<char*>(_AddressBuffer.GetBufferPtr()), TransfferedBytes,
				sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
				&LocalSockAddr, &RemoteAddrLength, &RemoteSockAddr, &RemoteAddrLength);

			const auto ListenDispatch = [&]()
			{
				_ListenCallbackDelegate(this, reinterpret_cast<IocpContextImpl*>(_ContextPtr)->CreateDescriptor(_Client), RemoteSockAddr,
					RemoteAddrLength, _Self);
			};

			Pool->EnqueueJob(ListenDispatch);
		};

		Process();

		AcceptRequest();
	}

	void Win32ListenerComponent::IOError(OVERLAPPED* pRawOverlapped, uint32_t LastError)
	{
		AcceptRequest();
	}

	void Win32ListenerComponent::IOTimeout(OVERLAPPED* pRawOverlapped)
	{
		AcceptRequest();
	}

	void Win32ListenerComponent::Disable()
	{
		closesocket(_Listener->GetDescriptor());
	}

	void Win32ListenerComponent::Free()
	{
		delete this;
	}

}

#endif