#include "MiniDump.h"
#include "DateTime.h"

MiniDump::MiniDump()
{
	::SetUnhandledExceptionFilter(exceptionFilter);
}


MiniDump::~MiniDump()
{
	
}

LONG WINAPI MiniDump::exceptionFilter(ExceptionPoint exceptionInfo)
{
	std::cout << "Creating Dump Files\n";
	_CrtMemDumpAllObjectsSince(nullptr);

	HMODULE dumpDll = LoadLibrary("DBGHELP.DLL");
	if (dumpDll == NULL)
	{
		std::cout << "Debug Help dll load failed.\n";
		Sleep(1000);
		return 0;
	}

	std::string dumpPatch;
	dumpPatch += Singleton<DateTime>::GetInstance()->nowTime(DATETIME_FORMAT);
	dumpPatch += ".dmp";

	HANDLE hFile = CreateFile(dumpPatch.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, nullptr,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		std::cout << "CreateFile Failed dumpPatch : " << dumpPatch << " ErrorCode : " << GetLastError() << std::endl;
		Sleep(1000);
		return 0;
	}

	_MINIDUMP_EXCEPTION_INFORMATION info;
	info.ThreadId = GetCurrentThreadId();
	info.ExceptionPointers = exceptionInfo;
	info.ClientPointers = false;

	
	WRITEDUMP dumpFunc = (WRITEDUMP)GetProcAddress(dumpDll, "MinidumpWriteDump");
	if (dumpFunc(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal,
		&info, nullptr, nullptr) == false)
	{
		std::cout << "WriteDump Failed\n";
		Sleep(1000);
		return 0;
	}

	CloseHandle(hFile);

	return EXCEPTION_CONTINUE_SEARCH;



	
}