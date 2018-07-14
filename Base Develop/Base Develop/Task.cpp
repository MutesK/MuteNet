#include "stdafx.h"
#include "Task.h"

Task::Task(dolamda _dolamda, const char* callerfunction,
	const char* callerfilename, const size_t& callerfileline)
	:_lambda(_dolamda), _callerfunction(callerfunction),
	_callerfile(callerfilename), _callerfileline(callerfileline)

{
	_isCheckedCompleteLambda = false;
}

Task::Task(dolamda _dolamda, completeNotifyDolamda completelambda,
	const char* callerfunction, const char* callerfilename,
	const size_t& callerfileline)
{
	_isCheckedCompleteLambda = true;
}

Task::~Task()
{
}


bool Task::IsEmptyCompeteLambda()
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