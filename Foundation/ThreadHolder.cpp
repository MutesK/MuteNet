#include "pch.h"
#include "ThreadHolder.h"

namespace Util
{
	ThreadHolder::ThreadHolder(const std::string& ThreadName)
		:_ThreadName(ThreadName)
	{
		_Thread = std::thread(std::mem_fn(&ThreadHolder::_DoWork), this);

		ChangeThreadName(_ThreadName);
	}

	void ThreadHolder::SetEvent()
	{
		_WorkSignal.Set();
	}

	void ThreadHolder::SetAllEvent()
	{
		_WorkSignal.SetAll();
	}

	void ThreadHolder::Finalize()
	{
		_ShutdownSignal.Setter(true);
		SetAllEvent();
	}

	void ThreadHolder::ChangeThreadName(const std::string& ThreadName)
	{
		static const DWORD MS_VC_EXCEPTION = 0x406D1388;

		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType;        // must be 0x1000
			LPCSTR szName;       // pointer to name (in same addr space)
			DWORD dwThreadID;    // thread ID (-1 caller thread)
			DWORD dwFlags;       // reserved for future use, most be zero
		} THREADNAME_INFO;

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = ThreadName.c_str();
		info.dwThreadID = GetThreadId(static_cast<HANDLE>(_Thread.native_handle()));
		info.dwFlags = 0;

		__try
		{
			RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
		}
	}

	void ThreadHolder::_DoWork()
	{
		while (!_ShutdownSignal.Getter())
		{
			_WorkSignal.Wait();

			DoWork();
		}
	}

}