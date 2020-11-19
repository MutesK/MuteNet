//
// Created by junmkim on 2020-11-19.
//


#include "Common.h"
#include "TypeDefine.hpp"
#include "Descriptor.h"
#include "SocketDescriptorHelper.hpp"

namespace MuteNet
{

    int SocketDescriptorHelper::InetPton(int af, const void *dest, size_t len)
    {
#ifdef _WIN32
#else
#endif

    }

    int SocketDescriptorHelper::SetSocketNonblock(descriptor_t fd)
    {
#ifdef _WIN32
        {
		unsigned long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
			event_sock_warn(fd, "ioctlsocket(%d, FIONBIO, &%lu)", (int)fd, (unsigned long)nonblocking);
			return -1;
		}
	}
#else
        {
            int flags;
            if ((flags = fcntl(fd, F_GETFL, NULL)) < 0) {
                return -1;
            }
            if (!(flags & O_NONBLOCK)) {
                if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
                    return -1;
                }
            }
        }
#endif
    }

    int SocketDescriptorHelper::SetListenSocketReuseable(descriptor_t listen)
    {
#if defined(SO_REUSEADDR) && !defined(_WIN32)
        int one = 1;
        return setsockopt(listen, SOL_SOCKET, SO_REUSEADDR, (void*) &one,
                          (socklen_t)sizeof(one));
#else
        return 0;
#endif
    }
}