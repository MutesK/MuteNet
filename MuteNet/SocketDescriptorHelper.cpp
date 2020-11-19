//
// Created by junmkim on 2020-11-19.
//


#include "Common.h"
#include "TypeDefine.hpp"
#include "Descriptor.h"
#include "SocketDescriptorHelper.hpp"

namespace MuteNet
{

    void SocketDescriptorHelper::InetPton(int af, const void *source, char* dest, size_t len)
    {
        if (af == AF_INET)
        {
            const auto* in = reinterpret_cast<const struct in_addr*>(source);
            const uint32_t addr = ntohl(in->S_un.S_addr);
            auto result = _snprintf_s(dest, len, len, "%d.%d.%d.%d",
                                      static_cast<int>(static_cast<uint8_t>((addr >> 24) & 0xff)),
                                      static_cast<int>(static_cast<uint8_t>((addr >> 16) & 0xff)),
                                      static_cast<int>(static_cast<uint8_t>((addr >> 8) & 0xff)),
                                      static_cast<int>(static_cast<uint8_t>((addr) & 0xff)));

        }
        else if (af == AF_INET6)
        {
            const auto* addr = reinterpret_cast<const struct in6_addr*>(source);
            char buf[64];

            int longestGapLen = 0, longestGapPos = -1, curGapPos = -1, curGapLen = 0;

            uint16_t words[8];
            for (int i = 0; i < 8; ++i)
            {
                words[i] =
                        (static_cast<uint16_t>(addr->s6_addr[2 * i]) << 8) + addr->s6_addr[2 * i + 1];
            }

            if (words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 &&
                words[4] == 0 && (words[5] == 0 && words[6] && words[7]) ||
                (words[5] == 0xffff))
            {
                if (words[5] == 0)
                {
                    _snprintf_s(buf, sizeof(buf), "::%x.%d.%d.%d", addr->s6_addr[12], addr->s6_addr[13],
                                                    addr->s6_addr[14], addr->s6_addr[15]);
                }
                else
                {
                    _snprintf_s(buf, sizeof(buf), "::%x:%d.%d.%d.%d", words[5],
                                addr->s6_addr[12], addr->s6_addr[13],
                                addr->s6_addr[14], addr->s6_addr[15]);
                }

                if (strlen(buf) > len)
                    return;

                strncpy_s(dest, len, buf, len);
            }
        }


    }

    int SocketDescriptorHelper::SetSocketNonblock(descriptor_t fd)
    {
#ifdef _WIN32
        {
		unsigned long nonblocking = 1;
		if (ioctlsocket(fd, FIONBIO, &nonblocking) == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
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