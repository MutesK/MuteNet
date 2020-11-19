//
// Created by Mute on 2020-11-15.
//

#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeDescriptor.hpp"
#include "UnixLikeIOContextImpl.hpp"

#ifdef POSIX_PLATFORM

namespace EventLoop
{
    UnixLikeDescriptor::UnixLikeDescriptor(RawIOContextImplPtr const &Ptr, socket_t Socket) :
            IDescriptor(Ptr, Socket)
    {

    }

    UnixLikeDescriptor::~UnixLikeDescriptor()
	{
        close(_descriptor);
	}
	
	void UnixLikeDescriptor::Read ( )
	{
	}
	
	void UnixLikeDescriptor::Write (void *data, size_t length )
	{
		{
			std::unique_lock<std::shared_mutex> lock ( _WriteBuffer._mutex );
			
			const auto InputedData = _WriteBuffer.PutData ( data, length );
			if ( InputedData != length )
			{
                std::overflow_error("Writebuffer is not enough Buffer");
			}
		}
	}

    void UnixLikeDescriptor::Enable()
    {

    }

    void UnixLikeDescriptor::Shutdown(uint16_t Flag)
    {
        shutdown(_descriptor, Flag);
    }
}
#endif