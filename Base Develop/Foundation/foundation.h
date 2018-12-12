#pragma once


#include <stdio.h>
#include <tchar.h>


#include <winsock2.h>
#include <ws2tcpip.h>

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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

#pragma comment(lib, "ws2_32.lib")


// Get, Set �Լ��� �ڵ����� ���������ش�.
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