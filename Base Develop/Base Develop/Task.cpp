#include "stdafx.h"
#include "Task.h"

Task::Task(dolamda _dolamda, const char* callerfunction,
	const char* callerfilename, const size_t& callerfileline)
	:_lambda(_dolamda), _callerfunction(callerfunction),
	_callerfile(callerfilename), _callerfileline(callerfileline)

{
	SetLambda(_dolamda, callerfunction, callerfilename, callerfileline);
}

Task::Task(dolamda _dolamda, completeNotifyDolamda completelambda,
	const char* callerfunction, const char* callerfilename,
	const size_t& callerfileline)
{
	SetCompleteLambda(_dolamda, completelambda,
		callerfunction, callerfilename, callerfileline);
}

Task::Task()
{
}

Task::~Task()
{
}

void Task::SetLambda(dolamda _dolamda, const char* callerfunction,
	const char* callerfilename, const size_t& callerfileline)
{
	_isCheckedCompleteLambda = false;

	_lambda = _dolamda;
	_callerfunction = callerfunction;
	_callerfile = callerfilename;
	_callerfileline = callerfileline;
}

void Task::SetCompleteLambda(dolamda _dolamda, completeNotifyDolamda completelambda,
	const char* callerfunction, const char* callerfilename,
	const size_t& callerfileline)
{
	_isCheckedCompleteLambda = true;

	_lambda = _dolamda;
	_completelambda = completelambda;
	_callerfunction = callerfunction;
	_callerfile = callerfilename;
	_callerfileline = callerfileline;
}

bool Task::IsEmptyCompleteLambda()
{
	return _isCheckedCompleteLambda;
}

void Task::Do()
{
	_lambda();
}

void Task::CompleteDo()
{
	_completelambda();
}