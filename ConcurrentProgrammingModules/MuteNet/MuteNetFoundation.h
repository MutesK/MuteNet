#pragma once

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")


#include <WinSock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>

#include "../ConcurrentProgrammingModules/CommonHeader.h"


static const DWORD AddressLength = sizeof(SOCKADDR_IN) + 16;
