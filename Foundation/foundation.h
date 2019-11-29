#pragma once

#define WIN32_LEAN_AND_MEAN             

#define NOMINMAX

#include <Windows.h>
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

#include <concurrent_unordered_map.h>
#include <shared_mutex>
#include <concurrent_queue.h>

#include <signal.h>
#include <cassert>
#include <sstream>

#include <cstdio>

#include "ObjectPool.h"
#include "MacroFunction.h"

using namespace std::chrono_literals;

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

	inline bool GetBitUInt16(const uint16_t& Flag, int Position)
	{
		return (Flag >> (Position - 1)) & 1;
	}
}


const size_t BUFFER_LENGTH = 20000;