#pragma once

#include <FoundationCommon.h>
#include <cassert>

#if defined(WIN32)

#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>

#include <wlanapi.h>
#include <iphlpapi.h>
#include <netioapi.h>

#pragma comment(lib, "ws2_32")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "Iphlpapi.lib")

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <zconf.h>

#define abstact
#endif

#if !defined(IOCONTEXT_KQUEUE) && !defined(IOCONTEXT_EPOLL) && !defined(IOCONTEXT_IOCP)
#define IOCONTEXT_SELECT
#endif

#if defined(IOCONTEXT_KQUEUE) || defined(IOCONTEXT_EPOLL) || defined(IOCONTEXT_SELECT)
#define POSIX_PLATFORM
#endif


