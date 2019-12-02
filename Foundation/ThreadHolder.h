#pragma once

#include "Event.h"
#include "foundation.h"

namespace Util
{
	class ThreadHolder
	{
		Event					_WorkSignal;
		Property<bool, false>	_ShutdownSignal;
		std::thread				_Thread;
		std::string				_ThreadName;

	public:
		ThreadHolder(const std::string& ThreadName);
		virtual void DoWork() = 0;

		void SetEvent();
		void SetAllEvent();

		void Finalize();

		void ChangeThreadName(const std::string& ThreadName);
	private:
		void _DoWork();

		NON_COPYABLE(ThreadHolder);
	};
}