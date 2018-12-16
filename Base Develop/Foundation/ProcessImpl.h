#pragma once

#include "foundation.h"

class Pipe;

class ProcessHandleImpl
{
public:
	ProcessHandleImpl(HANDLE hProcess, size_t PID);
	~ProcessHandleImpl();

	GET_CONST_ATTRIBUTE(HANDLE, hProcess);
	GET_CONST_ATTRIBUTE(size_t, pid);


	int Wait() const;
	void closeHandle();

private:
	HANDLE _hProcess;
	size_t _pid;
};

class ProcessImpl
{
public:
	using PID = unsigned int;
	using Args = std::vector<std::string>;
	using Envs = std::map<std::string, std::string>;

	// PID �����´�
	static PID id();
	// ���μ��� �Һ� �м�
	static void times(long& OUT userTime, long& OUT kernelTime);
	static std::shared_ptr<ProcessHandleImpl> Launch(const std::string& command,
		const Args& args, const std::string& initDirectory,
		Pipe* inPipe, Pipe* outPipe, Pipe* errPipe, const Envs& env);
	static void Kill(ProcessHandleImpl& Handle);
	static void Kill(PID pid);
	static bool isRunning(const ProcessHandleImpl& handle);
	static bool isRunning(PID pid);
};

