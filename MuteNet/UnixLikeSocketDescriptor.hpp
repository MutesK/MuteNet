//
// Created by Mute on 2020-11-15.
//

#ifndef MUTENET_UNIXLIKESOCKETDESCRIPTOR_HPP
#define MUTENET_UNIXLIKESOCKETDESCRIPTOR_HPP

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "SocketDescriptor.h"

namespace EventLoop
{
	class UnixLikeSocketDescriptor : public ISocketDescriptor
	{
	public:
        UnixLikeSocketDescriptor ( const RawIOContextImplPtr &Ptr, socket_t Socket );;
		virtual ~UnixLikeSocketDescriptor ( );
		
		virtual void Write ( void *data, size_t length ) override;
		
		virtual void Enable ( ) override;
		
		virtual void Disable ( uint16_t Flag ) override;
		
	private:
		virtual void Read ( ) override;
	};
}

#endif

#endif //MUTENET_UNIXLIKESOCKETDESCRIPTOR_HPP
