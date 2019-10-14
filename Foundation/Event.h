#pragma once
#include "SafeSharedLock.h"

namespace Util
{
	class Event
	{
		bool _shouldContinue;
		std::mutex _mutex;
		std::condition_variable  _condVar;
	public:
		Event();

		void Wait();
		void Set();
		void SetAll();
		bool Wait(unsigned int TimeoutMsec);
	};
}

