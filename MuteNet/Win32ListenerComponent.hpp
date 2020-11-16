//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_WIN32LISTENERCOMPONENT_HPP
#define MUTENET_WIN32LISTENERCOMPONENT_HPP

#include "ListenerComponent.hpp"
#include "WinDescriptor.hpp"
#include <OutputMemoryStream.h>

#if defined(WIN32)

namespace EventLoop
{
	class Win32ListenerComponent : public ListenerComponent, public IWinSocketDescriptor, public Util::IRunnable
	{
		bool                     _IsStop = false;
		Util::OutputMemoryStream _AddressBuffer;
		socket_t                 _ClientSocket;
		OVERLAPPED               _Overlapped;
		
		
		Win32ListenerComponent(const RawIOContextImplPtr &ContextEvent,
		                          ListenerComponent::CallbackDelegate &&Callback,
		                          void *Self, uint32_t Flag, int backlog, socket_t listenSocket);
		
		void AcceptRequest();
		
		friend class IocpContextImpl;
	public:
		virtual ~Win32ListenerComponent();
		
		virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) override;
		
		virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError ) override;
		
		virtual void IOTimeout ( OVERLAPPED *pRawOverlapped ) override;
		
		virtual void Start ( ) override;
		
		virtual void Stop ( ) override;
		
		virtual bool IsStop ( ) const override;
	};
}

#endif
#endif //MUTENET_WIN32LISTENERCOMPONENT_HPP
