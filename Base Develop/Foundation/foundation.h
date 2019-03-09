#pragma once


#include <stdio.h>
#include <tchar.h>
#include <cstdlib>

#include <winsock2.h>
#include <ws2tcpip.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <Windows.h>
#include <DbgHelp.h>
#include <process.h>

#include <string>
#include <iostream>

#include <functional>
#include <type_traits>

#include <memory>
#include <thread>
#include <mutex>
#include <map>
#include <unordered_map>
#include <queue>
#include <shared_mutex>

#include <cmath>
#include <new>

///////////////////
#include <vector>
#include <atomic>
#include <algorithm>
#include <set>
#include <stack>

#include <sstream>
#include <concurrent_queue.h>
#include <concurrent_unordered_map.h>

#include <sql.h>
#include <future>

#pragma comment(lib, "ws2_32.lib")


// Get, Set 함수를 자동으로 생성시켜준다.
#include "Property.h"
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

#define SHARED_LOCK(mutex)	\
	std::shared_lock<std::shared_mutex> lock(##mutex) 

#define UNIQUE_LOCK(mutex)	\
	std::unique_lock<std::shared_mutex> lock(##mutex)

#define WIN32_LEAN_AND_MEAN
#define MS_VC_EXCEPTION 0x406d1388

inline void TurnOnLowFragmentHeap() // LFH On
{
	ULONG HeapFragValue{ 0 };
	HANDLE Heaps[100];
	DWORD HeapCount = GetProcessHeaps(100, Heaps);

	for (size_t i = 0; i < HeapCount; i++)
	{
		HeapSetInformation(Heaps[i], HeapCompatibilityInformation, &HeapFragValue, sizeof(HeapFragValue));
	}
}
