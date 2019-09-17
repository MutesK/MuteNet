#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#define NOMINMAX

#include <Windows.h>
#include <iostream>
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
#include <concurrent_queue.h>

#include <signal.h>

#include "ObjectPool.h"

#define WIN32_LEAN_AND_MEAN

#define NON_COPYABLE(name)					\
name(const name&) = delete;					\
name(const name&&) = delete;				\
name& operator=(const name&) = delete;		\
name& operator=(const name&&) = delete;


#define THREAD_SHUTDOWN_FLAG()							\
private:												\
	std::atomic<bool>		_shutdownflag = false;		\
public:													\
	inline bool	GetShutdownThreadSignal() const			\
	{													\
		return _shutdownflag.load();					\
	}													\
	inline void SetShutdownThreadSignal(bool flag)		\
	{													\
		_shutdownflag.store(flag);						\
	}


#define GET_SET_ATTRIBUTE(TYPE, VALUE) \
	TYPE get_##VALUE()				   \
	{								   \
		return _##VALUE;			   \
	}								   \
	void set_##VALUE(##TYPE value)     \
	{								   \
		_##VALUE = value;		       \
	}

#define GET_CONST_ATTRIBUTE(TYPE, VALUE) \
	TYPE get_##VALUE() const	       \
	{								   \
		return _##VALUE;			   \
	}	

#define IN
#define OUT


namespace Util
{
	using Timestamp = std::chrono::steady_clock::time_point;


	inline bool ChangeThreadName(void* nativeHandle, const std::string& threadName)
	{
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType;        // must be 0x1000
			LPCSTR szName;       // pointer to name (in same addr space)
			DWORD dwThreadID;    // thread ID (-1 caller thread)
			DWORD dwFlags;       // reserved for future use, most be zero
		} THREADNAME_INFO;

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName.c_str();
		info.dwThreadID = GetThreadId(static_cast<HANDLE>(nativeHandle));
		info.dwFlags = 0;


		return true;
	}
}
