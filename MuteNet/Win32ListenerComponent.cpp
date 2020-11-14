//
// Created by junmkim on 2020-11-12.
//

#if defined(WIN32)

#include "Common.h"
#include "TypeDefine.hpp"
#include "Win32ListenerComponent.hpp"
#include "IocpContextImpl.hpp"

namespace EventLoop
{
	
	Win32ListenerComponent::Win32ListenerComponent ( RawIOContextImplPtr const &ContextEvent,
	                                                 ListenerComponent::CallbackDelegate &&Callback, void *Self,
	                                                 uint32_t Flag, int backlog, socket_t listenSocket )
			: ListenerComponent ( ContextEvent, std::move(Callback), Self, Flag, backlog, listenSocket )
	{
		auto Iocp = static_cast<IocpContextImpl>(_ContextPtr)->_IocpHandle;
		
		CreateIoCompletionPort ( reinterpret_cast<HANDLE>(listenSocket), Iocp,
		                         reinterpret_cast<ULONG_PTR>(this), 0 );
	}
	
	Win32ListenerComponent::~Win32ListenerComponent ( )
	{
	
	}
	
	void Win32ListenerComponent::AcceptRequest ( )
	{
		static auto& Extension = static_cast<IocpContextImpl>(_ContextPtr)->_Extensions;
		static AcceptExPtr& _AcceptExFunctionPtr = Extension._AcceptEx;
		static DWORD Pending = 0;
		
		_ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if(!_AcceptExFunctionPtr(_ListenSocket, _AddressBuffer.GetBufferPtr(), 0,
		                         sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Pending, &_Overlapped))
		{
		
		}
	}
	
	void Win32ListenerComponent::IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes )
	{
		static auto& Extension = static_cast<IocpContextImpl>(_ContextPtr)->_Extensions;
		static GetAcceptExSockAddrsPtr& _GetAcceptExSockAddrPtr = Extension._GetAcceptExSockaddrs;
		
		const static auto Process = [&]()
		{
			if ( nullptr == pRawOverlapped )
				return;
			
			
			if ( setsockopt ( _ClientSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			                  reinterpret_cast<char *>(&_ListenSocket), sizeof ( _ListenSocket )) == SOCKET_ERROR)
			{
			
			}
			
			SOCKADDR *RemoteSockAddr = nullptr, *LocalSockAddr = nullptr;
			int RemoteAddrLength = 0, LocalAddrLength = 0;
			
			_GetAcceptExSockAddrPtr ( const_cast<char *>(_AddressBuffer.GetBufferPtr ( )), TransfferedBytes,
			                          sizeof ( SOCKADDR_IN ) + 16, sizeof ( SOCKADDR_IN ) + 16,
			                          &LocalSockAddr, &RemoteAddrLength, &RemoteSockAddr, &RemoteAddrLength );
			
			const auto &Pool = _ContextPtr->GetThreadPool ( );
			static const auto ListenDispatch = [ & ] ( )
			{
				_ListenCallbackDelegate ( this, _ContextPtr->CreateSocket ( _ClientSocket ), RemoteSockAddr,
				                          RemoteAddrLength, _Self );
			};
			
			Pool->EnqueueJob ( ListenDispatch );
		};
		
		Process();
		
		if(!IsStop())
			AcceptRequest();
	}
	
	void Win32ListenerComponent::IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError )
	{
	
	}
	
	void Win32ListenerComponent::IOTimeout ( OVERLAPPED *pRawOverlapped )
	{
	
	}
	
	void Win32ListenerComponent::Start ( )
	{
		AcceptRequest();
	}
	
	void Win32ListenerComponent::Stop ( )
	{
		_IsStop = true;
	}
	
	bool Win32ListenerComponent::IsStop ( ) const
	{
		return _IsStop;
	}
	
}

#endif