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

#include <mutex>
#include <shared_mutex>

#include <signal.h>
#include <cassert>
#include <sstream>

#include <cstdio>
#include <fstream>
#include <queue>

#include <future>
#include <unordered_map>

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>
#include <concurrent_vector.h>

#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

