#pragma once


#include "../ConcurrentProgrammingModules/CommonHeader.h"
#include <shared_mutex>

/*
	순환대기 방지
*/
using ThreadOwnMutexMap = concurrency::concurrent_unordered_map<std::thread::id, bool>;

class SafeSharedLock final
{
public:
	SafeSharedLock() = default;
	~SafeSharedLock() = default;

	class OwnChecker
	{
	public:
		OwnChecker(ThreadOwnMutexMap& ownMap);
		~OwnChecker();
	private:
		std::thread::id _currentThreadID;
		ThreadOwnMutexMap& _refOwnMap;
	};

	NON_COPYABLE(SafeSharedLock);

	ThreadOwnMutexMap _ownMap;
	std::shared_mutex _lock;
};

	#define SAFE_SHAREDLOCK(mutex)									\
		SafeSharedLock::OwnChecker checker(mutex._ownMap);			\
		std::shared_lock<std::shared_mutex> lock(mutex._lock);								    
																	
	#define SAFE_UNIQUELOCK(mutex)									\
		SafeSharedLock::OwnChecker checker(mutex._ownMap);			\
		std::unique_lock<std::shared_mutex> lock(mutex._lock);						    
 