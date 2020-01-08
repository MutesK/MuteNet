#include "pch.h"
#include "MiniDump.h"

namespace Util
{
	MiniDump::MiniDump()
	{ 
		_CrtSetReportMode(_CRT_WARN, 0);
		_CrtSetReportMode(_CRT_ASSERT, 0);
		_CrtSetReportMode(_CRT_ERROR, 0);
	
		_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);

		// signal(SIGABRT, SignalHandler);
		// signal(SIGINT, SignalHandler);
		//signal(SIGILL, SignalHandler);
		//signal(SIGFPE, SignalHandler);
		//signal(SIGSEGV, SignalHandler);
		//signal(SIGTERM, SignalHandler);

		::SetUnhandledExceptionFilter(exceptionFilter);
	}

	LONG MiniDump::exceptionFilter(ExceptionPoint exceptionInfo)
	{
		static std::atomic_bool isAlreadyWritedDump = false;

		if (isAlreadyWritedDump)
			return 0;

		isAlreadyWritedDump = true;

		_CrtMemDumpAllObjectsSince(nullptr);

		std::cout << "Create Dump File \n";

		const HMODULE dumpDll = LoadLibrary("DBGHELP.DLL");
		if (dumpDll == nullptr)
		{
			return 0;
		}

		// 나중에 Server Name 추가할것.
		const std::string dumpPatch = "ApplicationName.dmp";

		HANDLE hFile = CreateFile(dumpPatch.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			return 0;
		}

		_MINIDUMP_EXCEPTION_INFORMATION info{};
		info.ThreadId = GetCurrentThreadId();
		info.ExceptionPointers = exceptionInfo;
		info.ClientPointers = false;


		const auto dump_func = reinterpret_cast<WriteDump>(GetProcAddress(dumpDll, "MiniDumpWriteDump"));
		if (!static_cast<bool>(dump_func(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &info,
		                                nullptr, nullptr)))
		{
			return 0;
		}

		CloseHandle(hFile);

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void MiniDump::Crash()
	{
		throw;
	}
	void MiniDump::SignalHandler(int Error)
	{
		Crash();
	}

	void MiniDump::VirtualCallErrorHandler()
	{
		Crash();
	}
}
