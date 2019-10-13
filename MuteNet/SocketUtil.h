#pragma once

#include "pch.h"

namespace MuteNet
{
	const char* inet_ntop(int af, const void* source, char *dest, size_t len)
	{
		if(af == AF_INET)
		{
			const auto* in = reinterpret_cast<const struct in_addr *>(source);
			const uint32_t addr = ntohl(in->S_un.S_addr);
			auto result = _snprintf_s(dest, len, len, "%d.%d.%d.%d",
				static_cast<int>(static_cast<uint8_t>((addr >> 24) & 0xff)),
				static_cast<int>(static_cast<uint8_t>((addr >> 16) & 0xff)),
				static_cast<int>(static_cast<uint8_t>((addr >> 8) & 0xff)),
				static_cast<int>(static_cast<uint8_t>((addr) & 0xff)));

			if (result < 0 || static_cast<size_t>(result) >= len)
				return nullptr;
			else
				return dest;
		}
		else if(af == AF_INET6)
		{
			const auto* addr = reinterpret_cast<const struct in6_addr*>(source);
			char buf[64];
			char* cp;

			int longestGapLen = 0, longestGapPos = -1, curGapPos = -1, curGapLen = 0;

			uint16_t words[8];
			for(int i=0; i<8; ++i)
			{
				words[i] =
					(static_cast<uint16_t>(addr->s6_addr[2 * i]) << 8) + addr->s6_addr[2 * i + 1];
			}

			if(words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0 &&
				words[4] == 0 && (words[5] == 0 && words[6] && words[7]) || 
				(words[5] == 0xffff))
			{
				if(words[5] == 0)
				{
					 // IPv4
					_snprintf_s(buf, sizeof(buf), "::%d.%d.%d.%d",
						addr->s6_addr[12], addr->s6_addr[13],
						addr->s6_addr[14], addr->s6_addr[15]);
				}
				else
				{
					_snprintf_s(buf, sizeof(buf), "::%x:%d.%d.%d.%d", words[5],
						addr->s6_addr[12], addr->s6_addr[13],
						addr->s6_addr[14], addr->s6_addr[15]);
				}

				if (strlen(buf) > len)
					return nullptr;

				strncpy_s(dest, len, buf, len);
				return dest;
			}
		}
	}

	inline int listen_socket_reuseable(intptr_t socket)
	{
		int one = 1;

		return SocketDelegateInvoker::Invoke(setsockopt, socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&one),
			static_cast<int>(sizeof(int)));
	}

	inline int make_socket_nonblocking(intptr_t socket)
	{
		unsigned int nonblocking = 1;

		return SocketDelegateInvoker::Invoke(ioctlsocket, FIONBIO, &nonblocking);
	}
	}
}