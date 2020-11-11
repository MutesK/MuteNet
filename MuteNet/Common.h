#pragma once

#include <FoundationCommon.h>

#if defined(WIN32)
#else

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define abstact
#endif
