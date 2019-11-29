#include "pch.h"
#include "Task.h"


using namespace Util;

Task::Task(DoLambda _dolamda, const char* callerfunction,
           const char* callerfilename, const size_t& callerfileline)
	: _DoLambda(_dolamda), _CallerFunction(callerfunction),
	_CallerFile(callerfilename), _CallerFileLine(callerfileline)

{
	SetLambda(_dolamda, callerfunction, callerfilename, callerfileline);
}

Task::Task(DoLambda _dolamda, CompleteNotifyDoLambda completelambda,
           const char* callerfunction, const char* callerfilename,
           const size_t& callerfileline)
{
	SetCompleteLambda(_dolamda, completelambda,
	                  callerfunction, callerfilename, callerfileline);
}

void Task::SetLambda(DoLambda _dolamda, const char* callerfunction,
                     const char* callerfilename, const size_t& callerfileline)
{
	_IsCheckedCompleteLambda = false;

	_DoLambda = _dolamda;
	_CallerFunction = callerfunction;
	_CallerFile = callerfilename;
	_CallerFileLine = callerfileline;
}

void Task::SetCompleteLambda(DoLambda _dolamda, CompleteNotifyDoLambda completelambda,
                             const char* callerfunction, const char* callerfilename,
                             const size_t& callerfileline)
{
	_IsCheckedCompleteLambda = true;

	_DoLambda = _dolamda;
	_CompleteLambda = completelambda;
	_CallerFunction = callerfunction;
	_CallerFile = callerfilename;
	_CallerFileLine = callerfileline;
}

bool Task::IsEmptyCompleteLambda() const
{
	return _IsCheckedCompleteLambda;
}

void Task::Do() const
{
	_DoLambda();
}

void Task::CompleteDo() const
{
	_CompleteLambda();
}
