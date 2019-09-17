#pragma once

#include <DbgHelp.h>

namespace Util
{
	using ExceptionPoint = struct _EXCEPTION_POINTERS*;

	using WriteDump = BOOL(WINAPI*)(
		_In_  HANDLE hProcess,
		_In_  DWORD ProcessId,
		_In_  HANDLE hFile,
		_In_  MINIDUMP_TYPE DumpType,
		_In_  PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
		_In_  PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
		_In_  PMINIDUMP_CALLBACK_INFORMATION CallbackParam
		);

	class MiniDump final
	{
		MiniDump();
		~MiniDump() = default;
		static LONG WINAPI exceptionFilter(ExceptionPoint exceptionInfo);
		static void Crash();
	};

}