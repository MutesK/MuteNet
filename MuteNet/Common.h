#pragma once

#include <FoundationCommon.h>

#if defined(WIN32)

#include <winsock2.h>
#include <mswsock.h>
#include <ws2ipdef.h>

#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define abstact
#endif