//
// Created by Mute on 2020-11-15.
//

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeDescriptor.hpp"
#include "UnixLikeIOContextImpl.hpp"

namespace EventLoop
{
    UnixLikeDescriptor::UnixLikeDescriptor(RawIOContextImplPtr const &Ptr, socket_t Socket) :
            IDescriptor(Ptr, Socket)
    {

    }

	UnixLikeDescriptor::~UnixLikeDescriptor ( )
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
				// Buffer isn't Enough
			}
		}
	}
	
	void UnixLikeDescriptor::Enable ( )
	{
        if(IsVaildCallback())
        {
            std::logic_error("");
        }

        static_cast<IUnixLikeIOContextImpl *>(_ContextPtr)->Enable(_descriptor);

	}
	
	void UnixLikeDescriptor::Disable (uint16_t Flag )
	{
        if(IsVaildCallback())
        {
            std::logic_error("");
        }

        static_cast<IUnixLikeIOContextImpl *>(_ContextPtr)->Disable(_descriptor);

	}
}
#endif