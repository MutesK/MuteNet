//
// Created by Mute on 2020-11-14.
//

#if defined(WIN32)

#include "Common.h"
#include "TypeDefine.hpp"
#include "IocpContextImpl.hpp"
#include "WinDescriptor.hpp"
#include "Win32ListenerComponent.hpp"

namespace EventLoop
{
	DescriptorPtr IocpContextImpl::CreateDescriptor ( descriptor_t descriptor )
	{
		const auto &Ptr = DescriptorPtr ( (IDescriptor *)new WinSocketDescriptor ( this, descriptor));
		
		CreateIoCompletionPort ( reinterpret_cast<HANDLE>(descriptor), _IocpHandle,
		                         reinterpret_cast<ULONG_PTR>(Ptr), 0 );
		return Ptr;
	}
	
	ListenerPtr
	IocpContextImpl::CreateListener ( ListenerComponent::CallbackDelegate &&Callback, void *Self,
	                                  descriptor_t listenSocket )
	{
		return ListenerPtr ((ListenerComponent *) new Win32ListenerComponent (this, std::move(Callback), Self, listenSocket ));
	}
	
	IocpContextImpl::IocpContextImpl ( IOContextEvent &Event, uint32_t NumOfWorkerThread, uint32_t Timeout )
			: IOContextImpl ( Event, NumOfWorkerThread, Timeout )
	{
		_IocpHandle = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE, nullptr, 0, 0 );
		if ( _IocpHandle == nullptr )
		{
			DWORD dwErrVal = GetLastError ( );
			std::error_code ec ( dwErrVal, std::system_category ( ));
			throw std::system_error ( ec, "IOCP Created Failed" );
		}
	}
	
	IocpContextImpl::~IocpContextImpl ( )
	{
		const auto Threads = GetThreadPool ( )->GetWorkerThreadCount ( );
		
		for ( int32_t index = 0; index <= Threads; ++index )
		{
			PostQueue ( nullptr );
		}
		
	}
	
	void IocpContextImpl::Start ( )
	{
		const auto &ThreadPool = GetThreadPool ( );
		const auto Threads = ThreadPool->GetWorkerThreadCount ( );
		
		const static auto WorkerRun = [ & ] ( )
		{
			DWORD byteTransferred = 0;
			LPOVERLAPPED lpOverlapped = nullptr;
			IWinSocketDescriptor *WinSocketDescriptor = nullptr;
			
			while ( !IsStop ( ))
			{
				lpOverlapped = nullptr;
				WinSocketDescriptor = nullptr;
				
				if ( !GetQueuedCompletionStatus ( _IocpHandle, &byteTransferred,
				                                  reinterpret_cast<PULONG_PTR>(&WinSocketDescriptor), &lpOverlapped,
				                                  _Timeout ))
				{
					const auto lastError = WSAGetLastError ( );
					
					if ( nullptr == WinSocketDescriptor )
					{
						continue;
					}
					
					
					if ( WAIT_TIMEOUT != lastError )
					{
						WinSocketDescriptor->IOError ( lpOverlapped, lastError );
						continue;
					}
					
					WinSocketDescriptor->IOTimeout ( lpOverlapped );
					continue;
				} else
				{
					if ( byteTransferred == 0 && WinSocketDescriptor == 0 && lpOverlapped == nullptr )
						continue;
					
					const auto lastError = WSAGetLastError ( );
					
					if ( nullptr == WinSocketDescriptor )
					{
						continue;
					}
					
					WinSocketDescriptor->IOCompletion ( lpOverlapped, byteTransferred );
				}
			}
		};
		
		
		for ( int32_t index = 0; index <= Threads; ++index )
		{
			ThreadPool->EnqueueJob ( WorkerRun );
		}
	}
	
	void IocpContextImpl::Stop ( )
	{
		_Stop = true;
	}
	
	bool IocpContextImpl::IsStop ( ) const
	{
		return _Stop;
	}
	
	bool IocpContextImpl::PostQueue ( void *Pointer )
	{
		if (!PostQueuedCompletionStatus(_IocpHandle, 0, (ULONG_PTR)Pointer, nullptr))
		{
			return false;
		}
		
		return true;
	}
	
	IocpContextImpl::RaIIWSA::RaIIWSA ( )
	{
		WSADATA wsaData;
		
		auto res = WSAStartup (MAKEWORD( 2, 2 ), &wsaData );
		if ( res != 0 )
		{
			int err = WSAGetLastError ( );
			std::error_code ec ( err, std::system_category ( ));
			throw std::system_error ( ec, "WSAStartup Failed" );
		}
	}
	
	IocpContextImpl::RaIIWSA::~RaIIWSA ( )
	{
		WSACleanup ( );
	}
	
	Extension::Extension ( )
	{
		const GUID acceptex = WSAID_ACCEPTEX;
		const GUID connectex = WSAID_CONNECTEX;
		const GUID getaccpetexsockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
		
		SOCKET serviceProvider = socket ( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		
		if ( serviceProvider == INVALID_SOCKET)
			return;
		
		_AcceptEx =
				reinterpret_cast<AcceptExPtr>(GetExtension ( serviceProvider, &acceptex ));
		_ConnectEx =
				reinterpret_cast<ConnectExPtr>(GetExtension ( serviceProvider, &connectex ));
		_GetAcceptExSockaddrs =
				reinterpret_cast<GetAcceptExSockAddrsPtr>(GetExtension ( serviceProvider, &getaccpetexsockaddrs ));
	}
	
	void *Extension::GetExtension ( socket_t socket, const GUID *FunctorPtr )
	{
		void *ptr = nullptr;
		DWORD bytes = 0;
		
		auto ret = std::invoke ( WSAIoctl, socket, SIO_GET_EXTENSION_FUNCTION_POINTER,
		                         ( GUID * ) FunctorPtr, sizeof ( *FunctorPtr ), &ptr, sizeof ( ptr ), &bytes, nullptr,
		                         nullptr );
		
		assert( ret != SOCKET_ERROR );
		
		return ptr;
	}
	
}

#endif