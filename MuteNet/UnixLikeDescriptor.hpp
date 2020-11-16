//
// Created by Mute on 2020-11-15.
//

#ifndef MUTENET_UNIXLIKEDESCRIPTOR_HPP
#define MUTENET_UNIXLIKEDESCRIPTOR_HPP

#if defined(APPLE) || defined(LINUX) || defined(UNIXLIKE)

#include "Descriptor.h"

namespace EventLoop
{
	class UnixLikeDescriptor : public IDescriptor
	{
	public:
        UnixLikeDescriptor (const RawIOContextImplPtr &Ptr, socket_t Socket );;
		virtual ~UnixLikeDescriptor ( );
		
		virtual void Write ( void *data, size_t length ) override;
		
		virtual void Enable ( ) override;
		
		virtual void Disable ( uint16_t Flag ) override;
	private:
		virtual void Read ( ) override;
	};
}

#endif

#endif //MUTENET_UNIXLIKEDESCRIPTOR_HPP
