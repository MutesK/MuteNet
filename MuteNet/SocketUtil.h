#pragma once

#include "pch.h"
#include "LinkImpl.h"

namespace MuteNet
{
	class SocketUtil
	{
	public:
		static const char* inet_ntop(int af, const void* source, char* dest, size_t len)
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

				if (result < 0 || static_cast<size_t>(result) >= len)
					return nullptr;
				else
					return dest;
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

			return nullptr;
		}

		static int ReUseable(intptr_t socket)
		{
			int one = 1;

			return std::invoke(setsockopt, socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char*>(&one), static_cast<int>(sizeof(int)));
		}

		static inline int NonBlockSocket(intptr_t socket)
		{
			unsigned int nonblocking = 1;

			return std::invoke(ioctlsocket, socket, FIONBIO,
				reinterpret_cast<u_long*>(&nonblocking));
		}

		static inline std::string ErrorString(int ErrorCode)
		{
			// Note gai_strerror is not threadsafe on windows

			char ErrorStr[GAI_STRERROR_BUFFER_SIZE + 1];

			int MsgLen = FormatMessageA(
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS |
				FORMAT_MESSAGE_MAX_WIDTH_MASK,
				nullptr,
				ErrorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				ErrorStr,
				sizeof(ErrorStr) - 1,
				nullptr
			);

			return std::string(ErrorStr, MsgLen);
		}

		static inline int Connect(const LinkImplPtr& linkPtr, const sockaddr* Ip, int size)
		{
			return ::connect(linkPtr->_Socket, Ip, size);
		}
	};
}