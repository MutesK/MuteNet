//
// Created by Mute on 2020-11-14.
//

#if defined(WIN32)

#include "Common.h"
#include "TypeDefine.hpp"
#include "WinDescriptor.hpp"
#include "IocpContextImpl.hpp"

namespace EventLoop
{
	WinSocketDescriptor::WinSocketDescriptor ( EventLoop::RawIOContextImplPtr const &Ptr, socket_t Socket )
			: IDescriptor ( Ptr, Socket )
	{
	
	}
	
	WinSocketDescriptor::~WinSocketDescriptor ( )
	{
		closesocket(_descriptor);
	}
	
	
	void WinSocketDescriptor::Read ( )
	{
        IncreaseCounter();

		WSABUF buf;
		buf.buf = nullptr;
		buf.len = 0;
		
		DWORD RecvBytes = 0;
		DWORD Flag = 0;
		
		if ( _descriptor == INVALID_SOCKET)
		{
		    DecreaseCounter();
            if(GetCounter() == 0)
            {
                _ExceptCallback(this, ECONNREFUSED, _Key);
                return;
            }
			return;
		}
		
		_RecvOverlapped = OVERLAPPED {};
		int ret = WSARecv ( _descriptor, &buf, 1, &RecvBytes, &Flag,
		                    reinterpret_cast<LPOVERLAPPED>(&_RecvOverlapped), nullptr );

		auto Error = WSAGetLastError();
		if ( ret == SOCKET_ERROR && Error != WSA_IO_PENDING && Error != WSAEWOULDBLOCK)
		{
            DecreaseCounter();
            if(GetCounter() == 0)
            {
                _ExceptCallback(this, Error, _Key);
            }
			return;
		}
	}
	
	void WinSocketDescriptor::Write ( void *data, size_t length )
	{
        IncreaseCounter();

		WSABUF buf[2];
		
		if ( _descriptor == INVALID_SOCKET)
		{
            DecreaseCounter();
            if(GetCounter() == 0)
            {
                _ExceptCallback(this, ECONNREFUSED, _Key);
                return;
            }
			return;
		}
		
		{
			std::unique_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
			
			const auto InputedData = _WriteBuffer.PutData ( data, length );
			if ( InputedData != length )
			{
                DecreaseCounter();
				std::overflow_error("Writebuffer is not enough Buffer");

                if(GetCounter() == 0)
                {
                    _ExceptCallback(this, ECONNREFUSED, _Key);
                    return;
                }
			}
			
			_WriteBuffer.GetWriteBufferAndLengths ( &buf[ 0 ].buf, ( uint32_t & ) buf[ 0 ].len,
			                                        &buf[ 1 ].buf, ( uint32_t & ) buf[ 1 ].len );
		}
		
		DWORD SendBytes = 0;
		DWORD Flag = 0;
		
		_SendOverlapped = OVERLAPPED {};
		int ret = WSASend ( _descriptor, buf, 2, &SendBytes, 0, &_SendOverlapped, nullptr );

        auto Error = WSAGetLastError();
        if ( ret == SOCKET_ERROR && Error != WSA_IO_PENDING && Error != WSAEWOULDBLOCK)
		{
            DecreaseCounter();
            if(GetCounter() == 0)
            {
                _ExceptCallback(this, Error, _Key);
            }
			return;
		}
	}
	
	void WinSocketDescriptor::Enable ( )
	{
		IncreaseCounter();
		
		Read();
		
		DecreaseCounter();
		
		if(GetCounter() == 0)
		{
            _ExceptCallback(this, ECONNREFUSED, _Key);
		}
	}
	
	void WinSocketDescriptor::Shutdown (uint16_t Flag )
	{
		shutdown(_descriptor, Flag);
	}
	
	void WinSocketDescriptor::IOCompletion ( OVERLAPPED *pRawOverlapped, uint32_t TransfferedBytes )
	{
			if ( nullptr == pRawOverlapped )
			{
				return;
			}
			
			if ( 0 == TransfferedBytes )
			{
                Shutdown(SD_BOTH);
			}
			
			if ( &_RecvOverlapped == pRawOverlapped )
			{
				if(!_Read ( ))
                {
				    _ExceptCallback(this, WSAGetLastError(), _Key);
                }
				else
                {
                    _ReadCallback(this, _Key);
                    Read();
                }
			}
			else if ( &_SendOverlapped == pRawOverlapped )
			{
				{
					std::shared_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
					
					_WriteBuffer.MoveReadPostion ( TransfferedBytes );
				}
				
				_WriteCallback ( this, _Key );
			}
			else
            {
                _ExceptCallback(this, 0, _Key);
            }

            DecreaseCounter();

            if(GetCounter() == 0)
            {
                _ExceptCallback(this, ECONNREFUSED, _Key);
            }
	}
	
	void WinSocketDescriptor::IOError ( OVERLAPPED *pRawOverlapped, uint32_t LastError )
	{
        _ExceptCallback(this, LastError, _Key);

        DecreaseCounter();

        if(GetCounter() == 0)
        {
            _ExceptCallback(this, ECONNREFUSED, _Key);
        }
	}
	
	void WinSocketDescriptor::IOTimeout ( OVERLAPPED *pRawOverlapped )
	{
        _ExceptCallback(this, 0, _Key);

        DecreaseCounter();

        if(GetCounter() == 0)
        {
            _ExceptCallback(this, ECONNREFUSED, _Key);
        }
	}

    int WinSocketDescriptor::write(descriptor_t descriptor, const char *ptr, size_t length)
    {
        return send(descriptor, ptr, length, 0);
    }

    int WinSocketDescriptor::read(descriptor_t descriptor, char *ptr, size_t length)
    {
        return recv(descriptor, ptr, length, 0);
    }

    bool WinSocketDescriptor::_Write()
    {
        return false;
    }

}

#endif