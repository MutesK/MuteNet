//
// Created by Mute on 2020-11-14.
//

#ifndef MUTENET_WINSOCKETDESCRIPTOR_HPP
#define MUTENET_WINSOCKETDESCRIPTOR_HPP

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
	
	class WinSocketDescriptor : public ISocketDescriptor, public IWinSocketDescriptor
	{
		OVERLAPPED _RecvOverlapped;
		OVERLAPPED _SendOverlapped;
	protected:
		WinSocketDescriptor ( const RawIOContextImplPtr &Ptr, socket_t Socket );
		
		friend class IocpContextImpl;
	
	public:
		virtual void Read ( ) override;
		
		virtual void Write ( void *data, size_t length ) override;
		
		virtual void Enable ( uint16_t Flag ) override;
		
		virtual void Disable ( uint16_t Flag ) override;
	
	private:
		virtual void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes ) override;
		
		virtual void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError ) override;
		
		virtual void IOTimeout ( OVERLAPPED *pRawOverlapped ) override;
	};
}

#endif


#endif //MUTENET_WINSOCKETDESCRIPTOR_HPP
