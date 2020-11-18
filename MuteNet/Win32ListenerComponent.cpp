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
	                                                 descriptor_t listenSocket )
			: ListenerComponent ( ContextEvent, std::move(Callback), Self, listenSocket )
	{
		auto Iocp = reinterpret_cast<IocpContextImpl *>(_ContextPtr)->_IocpHandle;
		
		CreateIoCompletionPort ( reinterpret_cast<HANDLE>(listenSocket), Iocp,
		                         reinterpret_cast<ULONG_PTR>(this), 0 );
	}
	
	Win32ListenerComponent::~Win32ListenerComponent ( )
	{
	
	}
	
	void Win32ListenerComponent::AcceptRequest ( )
	{
		static auto& Extension = reinterpret_cast<IocpContextImpl *>(_ContextPtr)->_Extensions;
		static AcceptExPtr& _AcceptExFunctionPtr = Extension._AcceptEx;
		static DWORD Pending = 0;
		
		_Client = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		
		if(!_AcceptExFunctionPtr(GetDescriptor(), _Client, const_cast<char *>(_AddressBuffer.GetBufferPtr()), 0,
		                         sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &Pending, &_Overlapped))
		{
			return;
		}
	}
	
	void Win32ListenerComponent::IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes )
	{
		static auto& Extension = reinterpret_cast<IocpContextImpl *>(_ContextPtr)->_Extensions;
		static GetAcceptExSockAddrsPtr& _GetAcceptExSockAddrPtr = Extension._GetAcceptExSockaddrs;
		
		const static auto Process = [&]()
		{
			if ( nullptr == pRawOverlapped )
				return;
			
			auto Listen = GetDescriptor();
			if ( setsockopt (_Client, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT,
			                  reinterpret_cast<char *>(&Listen), sizeof (descriptor_t)) == SOCKET_ERROR)
			{
				return;
			}
			
			SOCKADDR *RemoteSockAddr = nullptr, *LocalSockAddr = nullptr;
			int RemoteAddrLength = 0, LocalAddrLength = 0;
			
			_GetAcceptExSockAddrPtr ( const_cast<char *>(_AddressBuffer.GetBufferPtr ( )), TransfferedBytes,
			                          sizeof ( SOCKADDR_IN ) + 16, sizeof ( SOCKADDR_IN ) + 16,
			                          &LocalSockAddr, &RemoteAddrLength, &RemoteSockAddr, &RemoteAddrLength );
			
			const auto &Pool = reinterpret_cast<IocpContextImpl *>(_ContextPtr)->GetThreadPool ( );
			const auto ListenDispatch = [ & ] ( )
			{
				_ListenCallbackDelegate ( this, reinterpret_cast<IocpContextImpl *>(_ContextPtr)->CreateDescriptor ( _Client ), RemoteSockAddr,
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
		if(!IsStop())
			AcceptRequest();
	}
	
	void Win32ListenerComponent::IOTimeout ( OVERLAPPED *pRawOverlapped )
	{
		if(!IsStop())
			AcceptRequest();
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

    bool Win32ListenerComponent::_Read()
    {
        return false;
    }

    bool Win32ListenerComponent::_Write()
    {
        return false;
    }

    int Win32ListenerComponent::write(descriptor_t descriptor, const char *ptr, size_t length)
    {
        return 0;
    }

    int Win32ListenerComponent::read(descriptor_t descriptor, char *ptr, size_t length)
    {
        return 0;
    }

    void Win32ListenerComponent::Read()
    {

    }

    void Win32ListenerComponent::Write(void *data, size_t length)
    {

    }

    void Win32ListenerComponent::Enable()
    {

    }

    void Win32ListenerComponent::Shutdown(uint16_t Flag)
    {

    }

}

#endif