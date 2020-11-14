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
	class WinSocketDescriptor : public ISocketDescriptor
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
		void IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes );
		
		void IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError );
		
		void IOTimeout ( OVERLAPPED *pRawOverlapped );
	};
}

#endif


#endif //MUTENET_WINSOCKETDESCRIPTOR_HPP
