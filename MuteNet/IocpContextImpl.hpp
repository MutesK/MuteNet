//
// Created by Mute on 2020-11-14.
//

#ifndef MUTENET_IOCPCONTEXTIMPL_HPP
#define MUTENET_IOCPCONTEXTIMPL_HPP

#include "IoContextImpl.hpp"
#include <Runnable.hpp>

namespace EventLoop
{
	class IocpContextImpl : public IOContextImpl, public Util::IRunnable
	{
		class RaIIWSA final
		{
			WSADATA _Data;
		public:
			RaIIWSA ( );
			
			~RaIIWSA ( );
		};
		
		struct Extension
		{
			typedef LPFN_ACCEPTEX AcceptExPtr;
			typedef LPFN_CONNECTEX ConnectExPtr;
			typedef LPFN_GETACCEPTEXSOCKADDRS GetAcceptExSockAddrsPtr;
			
			AcceptExPtr _AcceptEx;
			ConnectExPtr _ConnectEx;
			GetAcceptExSockAddrsPtr _GetAcceptExSockaddrs;
			
			Extension ( );
			
			void *GetExtension ( socket_t socket, const GUID *FunctorPtr );
		};
		
		RaIIWSA _WsaData;
		Extension _Extensions;
		HANDLE _IocpHandle;
		std::atomic_bool _Stop;
	
	public:
		IocpContextImpl ( IOContextEvent &Event,
		                  const uint32_t NumOfWorkerThread, const uint32_t Timeout );
		
		virtual ~IocpContextImpl ( );
		
		virtual SocketPtr CreateSocket ( socket_t Socket );
		
		virtual ListenerPtr
		CreateListener ( ListenerComponent::CallbackDelegate &&Callback, void *Self, uint32_t Flag, int backlog,
		                 socket_t listenSocket ) override;
		
		void PostQueue ( void *Pointer );
		
		virtual void Start ( ) override;
		
		virtual void Stop ( ) override;
		
		virtual bool IsStop ( ) const override;
	};
	
}

#endif //MUTENET_IOCPCONTEXTIMPL_HPP
