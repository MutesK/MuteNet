//
// Created by Mute on 2020-11-14.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "WinSocketDescriptor.hpp"
#include "IocpContextImpl.hpp"

namespace EventLoop
{
	WinSocketDescriptor::WinSocketDescriptor ( EventLoop::RawIOContextImplPtr const &Ptr, socket_t Socket )
			: ISocketDescriptor ( Ptr, Socket )
	{
	
	}
	
	
	void WinSocketDescriptor::Read ( )
	{
		WSABUF buf;
		buf.buf = nullptr;
		buf.len = 0;
		
		DWORD RecvBytes = 0;
		DWORD Flag = 0;
		
		if ( _socket == INVALID_SOCKET)
		{
		
		}
		
		_RecvOverlapped = OVERLAPPED {};
		int ret = WSARecv ( _socket, &buf, 1, &RecvBytes, &Flag,
		                    reinterpret_cast<LPOVERLAPPED>(&_RecvOverlapped), nullptr );
		
		if ( ret == SOCKET_ERROR)
		{
		
		}
	}
	
	void WinSocketDescriptor::Write ( void *data, size_t length )
	{
		WSABUF buf[2];
		
		if ( _socket == INVALID_SOCKET)
		{
		
		}
		
		{
			std::unique_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
			
			const auto InputedData = _WriteBuffer.PutData ( data, length );
			if ( InputedData != length )
			{
				// Buffer isn't Enough
			}
			
			_WriteBuffer.GetWriteBufferAndLengths ( &buf[ 0 ].buf, ( uint32_t & ) buf[ 0 ].len,
			                                        &buf[ 1 ].buf, ( uint32_t & ) buf[ 1 ].len );
		}
		
		DWORD SendBytes = 0;
		DWORD Flag = 0;
		
		_SendOverlapped = OVERLAPPED {};
		int ret = WSASend ( _socket, buf, 2, &SendBytes, 0, &_SendOverlapped, nullptr );
		if ( ret == SOCKET_ERROR)
		{
		
		}
	}
	
	void WinSocketDescriptor::Enable ( uint16_t Flag )
	{
	
	}
	
	void WinSocketDescriptor::Disable ( uint16_t Flag )
	{
	
	}
	
}