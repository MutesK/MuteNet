//
// Created by Mute on 2020-11-14.
//

#ifndef MUTENET_IOCPCONTEXTIMPL_HPP
#define MUTENET_IOCPCONTEXTIMPL_HPP

#if defined(WIN32)

#include "IoContextImpl.hpp"
#include <Runnable.hpp>

namespace EventLoop
{
	typedef LPFN_ACCEPTEX AcceptExPtr;
	typedef LPFN_CONNECTEX ConnectExPtr;
	typedef LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrsPtr;
	
	struct Extension
	{
		AcceptExPtr _AcceptEx;
		ConnectExPtr _ConnectEx;
		GetAcceptExSockAddrsPtr _GetAcceptExSockaddrs;
		
		Extension ( );
		
		void *GetExtension ( socket_t socket, const GUID *FunctorPtr );
	};
	
	class IocpContextImpl : public IOContextImpl, public Util::IRunnable
	{
		class RaIIWSA final
		{
			WSADATA _Data;
		public:
			RaIIWSA ( );
			
			~RaIIWSA ( );
		};
		
		RaIIWSA _WsaData;
		Extension _Extensions;
		HANDLE _IocpHandle;
		std::atomic_bool _Stop;
	
		friend class Win32ListenerComponent;
	public:
		IocpContextImpl ( IOContextEvent &Event,
		                  const uint32_t NumOfWorkerThread, const uint32_t Timeout );
		
		virtual ~IocpContextImpl ( );
		
		virtual DescriptorPtr CreateDescriptor ( descriptor_t Socket );
		
		virtual ListenerPtr
		CreateListener ( ListenerComponent::CallbackDelegate &&Callback, void *Self,
		                 socket_t listenSocket ) override;
		
		bool PostQueue ( void *Pointer );
		
		virtual void Start ( ) override;
		
		virtual void Stop ( ) override;
		
		virtual bool IsStop ( ) const override;
	};
	
}

#endif

#endif //MUTENET_IOCPCONTEXTIMPL_HPP
