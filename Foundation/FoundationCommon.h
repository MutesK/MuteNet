#pragma once

#include <iostream>
#include <atomic>
#include <vector>
#include <stdint.h>

#include <map>
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

#include "MacroFunction.h"


#if defined(WIN32)

#pragma warning(disable: 4717)  // StringFormatter Recursive Unpacking

#elif defined(__APPLE__) || defined(__linux__)

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_unordered_map.h>

namespace concurrency = tbb;
namespace Concurrency = tbb;

#define abstract

#endif