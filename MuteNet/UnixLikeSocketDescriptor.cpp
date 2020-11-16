//
// Created by Mute on 2020-11-15.
//

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "Common.h"
#include "TypeDefine.hpp"
#include "UnixLikeSocketDescriptor.hpp"
#include "UnixLikeIOContextImpl.hpp"

namespace EventLoop
{
    UnixLikeSocketDescriptor::UnixLikeSocketDescriptor(RawIOContextImplPtr const &Ptr, socket_t Socket) :
            ISocketDescriptor(Ptr, Socket)
    {

    }

	UnixLikeSocketDescriptor::~UnixLikeSocketDescriptor ( )
	{
		static_cast<IUnixLikeIOContextImpl *>(_ContextPtr)->_Container.Erase(reinterpret_cast<ISocketDescriptor *>(this));
	}
	
	void UnixLikeSocketDescriptor::Read ( )
	{
	}
	
	void UnixLikeSocketDescriptor::Write ( void *data, size_t length )
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
	
	void UnixLikeSocketDescriptor::Enable ( )
	{
        if(IsVaildSocket())
        {
            std::logic_error("");
        }

		static_cast<IUnixLikeIOContextImpl *>(_ContextPtr)->Enable(_socket);
	}
	
	void UnixLikeSocketDescriptor::Disable ( uint16_t Flag )
	{
        if(IsVaildSocket())
        {
            std::logic_error("");
        }


        static_cast<IUnixLikeIOContextImpl *>(_ContextPtr)->Disable(_socket);
	}
}
#endif