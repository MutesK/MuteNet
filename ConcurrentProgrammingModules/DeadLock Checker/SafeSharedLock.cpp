#include "SafeSharedLock.h"


SafeSharedLock::OwnChecker::OwnChecker(ThreadOwnMutexMap& Mapping)
	:_refOwnMap(Mapping)
{
	_currentThreadID = std::this_thread::get_id();

	if (true == _refOwnMap[_currentThreadID])
	{
		std::abort();
	}

	_refOwnMap[_currentThreadID] = true;
}

SafeSharedLock::OwnChecker::~OwnChecker()
{
	_refOwnMap[_currentThreadID] = false;
}

