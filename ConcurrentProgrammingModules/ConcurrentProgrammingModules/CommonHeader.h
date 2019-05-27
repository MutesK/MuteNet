#pragma once

#include <iostream>
#include <Windows.h>
#include <atomic>
#include <vector>

#include <map>
#include <list>

#include <thread>
#include <functional>
#include <algorithm>
#include <string>

#include <ctime>
#include <chrono>

#include <concurrent_unordered_map.h>
#include <shared_mutex>

#define WIN32_LEAN_AND_MEAN


#define NON_COPYABLE(name)					\
name(const name&) = delete;					\
name(const name&&) = delete;				\
name& operator=(const name&) = delete;		\
name& operator=(const name&&) = delete;


#define THREAD_SHUTDOWN_FLAG()							\
private:												\
	std::atomic<bool>		_shutdownflag = false;		\
	inline bool	GetShutdownThreadSignal() const			\
	{													\
		return _shutdownflag.load();					\
	}													\
	inline void SetShutdownThreadSignal(bool flag)		\
	{													\
		_shutdownflag.store(flag);						\
	}

using Timestamp = std::chrono::steady_clock::time_point;

