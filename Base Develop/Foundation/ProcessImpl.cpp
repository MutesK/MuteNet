#include "ProcessImpl.h"
#include "Exception.h"
#include "Pipe.h"



ProcessHandleImpl::ProcessHandleImpl(HANDLE hProcess, size_t PID)
	:_hProcess(hProcess), _pid(PID)
{
}


ProcessHandleImpl::~ProcessHandleImpl()
{
	closeHandle();
}


void ProcessHandleImpl::closeHandle()
{
	if (_hProcess != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_hProcess);
		_hProcess = INVALID_HANDLE_VALUE;
	}
}

int ProcessHandleImpl::Wait() const
{
	DWORD ret = WaitForSingleObject(_hProcess, INFINITE);
	if (ret != WAIT_OBJECT_0)
		throw SystemException("Wait Failed for Process");

	DWORD exitCode;
	if (GetExitCodeProcess(_hProcess, &exitCode) == false)
		throw SystemException("Can't get Exit Code For Process");

	return exitCode;
}

ProcessImpl::PID ProcessImpl::id()
{
	::GetCurrentProcessId();
}

void ProcessImpl::times(long& OUT userTime, long& OUT kernelTime)
{
	FILETIME Create;
	FILETIME Exit;
	FILETIME Kernel;
	FILETIME User;

	if (GetProcessTimes(GetCurrentProcess(), &Create, &Exit, &Kernel, &User) != 0)
	{
		ULARGE_INTEGER time;
		time.LowPart = Kernel.dwLowDateTime;
		time.HighPart = Kernel.dwHighDateTime;

		kernelTime = static_cast<long>(time.QuadPart / 10000000L);

		time.LowPart = User.dwLowDateTime;
		time.HighPart = User.dwHighDateTime;

		userTime = static_cast<long>(time.QuadPart / 10000000L);
	}
	else
	{
		kernelTime = userTime = -1;
	}
}


static bool argumentChecker(const std::string& arg)
{
	bool containsQuotable = std::string::npos != arg.find_first_of(" \t\n\v\"");
	bool isAlreadyQuoted = arg.size() > 1 && '\"' == arg[0] && '\"' == arg[arg.size() - 1];
	return containsQuotable && !isAlreadyQuoted;
}
static std::string escapeArgument(const std::string& arg)
{
	if (argumentChecker(arg))
	{
		std::string command("\"");

		for (std::string::const_iterator iter = arg.begin(); ; ++iter)
		{
			size_t backslash = 0;

			while (iter != arg.end() && '\\' == *iter)
			{
				++iter;
				++backslash;
			}

			if (iter == arg.end())
			{
				command.append(2 * backslash, '\\');
				break;
			}
			else if (*iter == '"')
			{
				command.append(2 * backslash + 1, '\\');
				command.push_back('"');
			}
			else
			{
				command.append(backslash, '\\');
				command.push_back(*iter);
			}
		}

		command.push_back('"');
		return command;
	}

	return arg;
}

// ¹Ì¿Ï¼º
std::shared_ptr<ProcessHandleImpl> ProcessImpl::Launch(const std::string& command,
	const ProcessImpl::Args& args, const std::string& initDirectory,
	Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Envs& env)
{
	std::string ArgumentLine = escapeArgument(command);
	for (Args::const_iterator iter = args.begin(); iter != args.end(); ++iter)
	{
		ArgumentLine.append(" ");
		ArgumentLine.append(escapeArgument(*iter));
	}

	if (ArgumentLine.size() > MAX_PATH)
	{
		// Path
	}

	STARTUPINFO startupInfo;
	GetStartupInfo(&startupInfo);
	startupInfo.cb = sizeof(STARTUPINFO);
	startupInfo.lpReserved = nullptr;
	startupInfo.lpDesktop = nullptr;
	startupInfo.lpTitle = nullptr;
	startupInfo.dwFlags = STARTF_FORCEOFFFEEDBACK;
	startupInfo.cbReserved2 = 0;
	startupInfo.lpReserved2 = nullptr;

	HANDLE hProcess = GetCurrentProcess();
	bool InheritHandle = false;
	if (inPipe != nullptr)
	{
		DuplicateHandle(hProcess, inPipe->readPipe(), hProcess, &startupInfo.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else if (GetStdHandle(STD_INPUT_HANDLE))
	{
		DuplicateHandle(hProcess, GetStdHandle(STD_INPUT_HANDLE), hProcess, &startupInfo.hStdInput, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else
	{
		startupInfo.hStdInput = 0;
	}

	if (outPipe != nullptr)
	{
		DuplicateHandle(hProcess, inPipe->writePipe(), hProcess, &startupInfo.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else if (GetStdHandle(STD_OUTPUT_HANDLE))
	{
		DuplicateHandle(hProcess, GetStdHandle(STD_OUTPUT_HANDLE), hProcess, &startupInfo.hStdOutput, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else
	{
		startupInfo.hStdOutput = 0;
	}


	if (errPipe)
	{
		DuplicateHandle(hProcess, errPipe->writePipe(), hProcess, &startupInfo.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else if (GetStdHandle(STD_ERROR_HANDLE))
	{
		DuplicateHandle(hProcess, GetStdHandle(STD_ERROR_HANDLE), hProcess, &startupInfo.hStdError, 0, TRUE, DUPLICATE_SAME_ACCESS);
		InheritHandle = true;
	}
	else
	{
		startupInfo.hStdError = 0;
	}

	if (InheritHandle)
	{
		startupInfo.dwFlags |= STARTF_USESTDHANDLES;
	}

	// 
}