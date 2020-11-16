//
// Created by Mute on 2020-11-14.
//

#ifndef MUTENET_WINDESCRIPTOR_HPP
#define MUTENET_WINDESCRIPTOR_HPP

#if defined(WIN32)

#include <cstddef>
#include <cstdint>
#include "SocketDescriptor.h"


namespace EventLoop
{
	class IWinSocketDescriptor
	{
	public:
		virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) = 0;
		
		virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError )= 0;
		
		virtual void IOTimeout ( OVERLAPPED *pRawOverlapped )= 0;
	};
	
	class WinSocketDescriptor : public IDescriptor, public IWinSocketDescriptor
	{
		OVERLAPPED _RecvOverlapped;
		OVERLAPPED _SendOverlapped;
	protected:
		WinSocketDescriptor ( const RawIOContextImplPtr &Ptr, socket_t Socket );
		~WinSocketDescriptor();
		friend class IocpContextImpl;
	
	public:
		virtual void Enable ( ) override;
		
		virtual void Disable ( uint16_t Flag ) override;
		
		virtual void Write ( void *data, size_t length ) override;
	
	private:
		virtual void Read ( ) override;
		
		virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) override;
		
		virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError ) override;
		
		virtual void IOTimeout ( OVERLAPPED *pRawOverlapped ) override;
	};
}

#endif


#endif //MUTENET_WINDESCRIPTOR_HPP
