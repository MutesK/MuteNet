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
	class Win32ListenerComponent : public ListenerComponent, public IWinDescriptor, public Util::IRunnable
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
        virtual bool _Read() override;

        virtual bool _Write() override;

        virtual int write(descriptor_t descriptor, const char *ptr, size_t length) override;

        virtual int read(descriptor_t descriptor, char *ptr, size_t length) override;

        virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) override;

        virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError ) override;

        virtual void IOTimeout ( OVERLAPPED *pRawOverlapped ) override;

        virtual void Start ( ) override;

        virtual void Stop ( ) override;

        virtual bool IsStop ( ) const override;

        virtual void Read() override;

        virtual void Write(void *data, size_t length) override;

        virtual void Enable() override;

        virtual void Disable(uint16_t Flag) override;
    private:

        friend class IocpContextImpl;
	public:
		virtual ~Win32ListenerComponent();

    };
}

#endif
#endif //MUTENET_WIN32LISTENERCOMPONENT_HPP
