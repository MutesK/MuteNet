#pragma once


#include "../Foundation/foundation.h"
#include "../Foundation/Singleton.hpp"
#include "../Foundation/ObjectPoolTLS.h"
#include "../Foundation/Thread.h"
#include "../Foundation/TaskManager.h"
#include "TcpSocket.h"
#include "UdpSocket.h"
#include "SocketUtil.h"
#include "SocketAddress.h"

using LinkHandle = size_t;
