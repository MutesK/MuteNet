#include "pch.h"
#include "MiniDump.h"

namespace Util
{
	MiniDump::MiniDump()
	{
		::SetUnhandledExceptionFilter(exceptionFilter);
	}

	LONG MiniDump::exceptionFilter(ExceptionPoint exceptionInfo)
	{
		_CrtMemDumpAllObjectsSince(nullptr);

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
		if (!static_cast<bool>(dump_func(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &info,
		                                nullptr, nullptr)))
		{
			return 0;
		}

		CloseHandle(hFile);

		return EXCEPTION_CONTINUE_SEARCH;
	}

	void MiniDump::Crash()
	{
		raise(SIGSEGV);
	}
}
