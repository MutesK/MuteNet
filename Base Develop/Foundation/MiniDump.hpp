
#include "MiniDump.h"

MiniDump::MiniDump()
{
	::SetUnhandledExceptionFilter(exceptionFilter);
}


MiniDump::~MiniDump()
{
	
}

LONG WINAPI MiniDump::exceptionFilter(ExceptionPoint exceptionInfo)
{

	_CrtMemDumpAllObjectsSince(nullptr);

	HMODULE dumpDll = LoadLibrary("DBGHELP.DLL");
	if (dumpDll == NULL)
	{
		return 0;
	}
	

	// 나중에 Server Name 추가할것.
	std::string dumpPatch;
	dumpPatch += Singleton<DateTime>::GetInstance()->nowTime(DATETIME_FORMAT);
	dumpPatch += ".dmp";

	HANDLE hFile = CreateFile(dumpPatch.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}

	_MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = exceptionInfo;
	info.ClientPointers = false;

	
	WRITEDUMP dumpFunc = (WRITEDUMP)GetProcAddress(dumpDll, "MiniDumpWriteDump");
	if (dumpFunc(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal,
		&info, nullptr, nullptr) == false)
	{
		return 0;
	}

	CloseHandle(hFile);

	return EXCEPTION_CONTINUE_SEARCH;

}

void MiniDump::Crash()
{
	int *p = 0;
	*p = 1;
}

static MiniDump dump;