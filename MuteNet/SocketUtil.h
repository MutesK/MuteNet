#pragma once

#include "pch.h"
#include "LinkImpl.h"
#include <winternl.h>
#include <netioapi.h>

using namespace Util;
namespace MuteNet
{
	class SocketUtil
	{
	public:
		static const char* InetNtop(int af, const void* source, char* dest, size_t len)
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
						auto str = StringHelper::Format("::{0}.{1}.{2}.{3}", addr->s6_addr[12], addr->s6_addr[13],
							addr->s6_addr[14], addr->s6_addr[15]);
						strcpy_s(buf, 64, str.c_str());
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

		static int ParseSockaddrPort(const std::string ip, struct sockaddr* OUT address, int* outLength)
		{
			char buffer[128];

			memset(address, 0, sizeof(address));

			const char* cp = strchr(ip.c_str(), ':');
			char* addressStr;
			char* portStr = nullptr;
			bool isIpv6 = false;

			if (ip[0] == '[')
			{
				cp = strchr(ip.c_str(), ']');

				if (cp == nullptr)
					return -1;

				auto len = cp - ip.c_str() + 1;
				if (len > sizeof(buffer) - 1) // ?
					return -1;

				memcpy(buffer, ip.c_str() + 1, len);
				buffer[len] = '\0';
				addressStr = buffer;

				if (cp[1] == ':')
					portStr = const_cast<char*>(cp) + 2;

				isIpv6 = true;
			}
			else if (cp && strchr(cp + 1, ':'))
			{
				isIpv6 = true;
				addressStr = const_cast<char *>(ip.c_str());
				portStr = nullptr;
			}
			else if (cp != nullptr)
			{
				isIpv6 = false;
				if (cp - ip.c_str() > sizeof(buffer) - 1)
				{
					return -1;
				}

				memcpy(buffer, ip.c_str(), cp - ip.c_str());
				buffer[cp - ip.c_str()] = '\0';
				addressStr = buffer;
				portStr = const_cast<char *>(cp) + 1;
			}
			else
			{
				addressStr = const_cast<char *>(ip.c_str());
				isIpv6 = false;
			}

			uint16_t port = 0;
			if (portStr)
			{
				port = atoi(portStr);
				if (port <= 0 || port > 65535) 
					return -1;
			}

			if (addressStr == nullptr)
				return -1;


			if (isIpv6)
			{
				struct sockaddr_in6 addrin6 { 0 };

				addrin6.sin6_family = AF_INET6;
				addrin6.sin6_port = htons(port);
				uint32_t ifIndex;
				if (1 != InetPtonScope(AF_INET6, addressStr, &addrin6.sin6_addr,
					&ifIndex))
					return -1;

				if (static_cast<int32_t>(sizeof(addrin6)) > * outLength)
					return -1;

				addrin6.sin6_scope_id = ifIndex;
				memcpy(address, &addrin6, sizeof(addrin6));
				*outLength = sizeof(addrin6);
				return 0;
			}

			struct sockaddr_in addr{0};
			addr.sin_family = AF_INET;
			addr.sin_port = htons(port);

			if (1 != InetPton(AF_INET, addressStr, &addr.sin_addr))
				return -1;

			if (static_cast<int>(sizeof(addr)) > * outLength)
				return -1;

			memcpy(address, &addr, sizeof(addr));
			*outLength = sizeof(addr);

			return 0;


		}

		static int InetPtonScope(int AddressFamily, const char* source,
			void* dest, uint32_t* IndexPtr)
		{
			*IndexPtr = 0;  

			if (AddressFamily != AF_INET6)
				return InetPton(AddressFamily, source, dest);

			char* cp = strchr(const_cast<char *>(source), '%');

			// IPv6에 Zone Index가 없다면
			if (cp == nullptr)
				return InetPton(AddressFamily, source, dest);

			auto NetworkInterfaceIndex = if_nametoindex(cp + 1);
			if (NetworkInterfaceIndex == 0)
			{
				char* check;
				NetworkInterfaceIndex = strtoul(cp + 1, &check, 10);
				if (check[0] != '\0')
					return 0;
			}

			*IndexPtr = NetworkInterfaceIndex;
			char* tempSource = _strdup(source);
			cp = strchr(tempSource, '%');
			*cp = '\0';

			int ret = InetPton(AddressFamily, tempSource, dest);

			return ret;
 		}

		static int InetPton(int AddressFamily, const char* source, void* dest)
		{
			return ::inet_pton(AddressFamily, source, dest);
		}

		static int GetDomainNameInfo(const sockaddr* Address, int Length,
			std::string& OUT HostName, std::string& OUT ServerInfo)
		{
			char tempHostName[NI_MAXHOST];
			char tempServInfo[NI_MAXSERV];

			auto ret = ::getnameinfo(Address, Length, tempHostName, sizeof(tempHostName),
				tempServInfo, sizeof(tempServInfo), 0);

			HostName = tempHostName;
			ServerInfo = tempServInfo;

			return ret;

		}
	};
}