//
// Created by junmkim on 2020-11-12.
//

#ifndef MUTENET_WIN32LISTENERCOMPONENT_HPP
#define MUTENET_WIN32LISTENERCOMPONENT_HPP

#include "ListenerComponent.hpp"
#include "WinDescriptor.hpp"
#include "IoContextImpl.hpp"
#include "OutputMemoryStream.h"

#if defined(WIN32)

namespace EventLoop
{
	class Win32ListenerComponent : public ListenerComponent, public IWinDescriptor
	{
		bool                     _IsStop = false;
		Util::OutputMemoryStream _AddressBuffer;
		OVERLAPPED               _Overlapped;
		descriptor_t			 _Client;
		
		Win32ListenerComponent(const RawIOContextImplPtr &ContextEvent,
		                          ListenerComponent::CallbackDelegate &&Callback,
		                          void *Self, descriptor_t listenSocket);
		
		void AcceptRequest();

    protected:
        virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) override;

        virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError ) override;

        virtual void IOTimeout ( OVERLAPPED *pRawOverlapped ) override;
    private:

        friend class IocpContextImpl;
	public:
		virtual ~Win32ListenerComponent();

        virtual void Disable() override;

        virtual void Free() override;

    };

}

#endif
#endif //MUTENET_WIN32LISTENERCOMPONENT_HPP
