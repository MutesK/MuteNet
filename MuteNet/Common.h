#pragma once

#pragma once

#include <iostream>
#include <atomic>
#include <vector>
#include <stdint.h>

#include <map>
#include <set>
#include <list>

#include <thread>
#include <functional>
#include <algorithm>
#include <string>

#include <ctime>
#include <chrono>

#include <shared_mutex>

#include <signal.h>
#include <cassert>
#include <sstream>

#include <cstdio>
#include <fstream>
#include <queue>

#include <future>
#include <unordered_map>

#include "MacroFunction.h"


#if defined(WIN32)

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>

#elif defined(__APPLE__) || defined(__linux__)

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>


namespace concurrency = tbb;
namespace Concurrency = tbb;

#define abstract

#endif

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

#include <netdb.h>
#include <net/if.h>

#define abstact
#endif

#if !defined(IOCONTEXT_KQUEUE) && !defined(IOCONTEXT_EPOLL) && !defined(IOCONTEXT_IOCP)
#define IOCONTEXT_SELECT
#endif

#if defined(IOCONTEXT_KQUEUE) || defined(IOCONTEXT_EPOLL) || defined(IOCONTEXT_SELECT)
#define POSIX_PLATFORM
#endif


