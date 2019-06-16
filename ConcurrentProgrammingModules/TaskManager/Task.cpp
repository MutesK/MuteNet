#include "Task.h"


using namespace Util;

Task::Task(DoLambda _dolamda, const char* callerfunction,
           const char* callerfilename, const size_t& callerfileline)
	: _lambda(_dolamda), _callerFunction(callerfunction),
	_callerFile(callerfilename), _callerFileLine(callerfileline)

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
	_isCheckedCompleteLambda = false;

	_lambda = _dolamda;
	_callerFunction = callerfunction;
	_callerFile = callerfilename;
	_callerFileLine = callerfileline;
}

void Task::SetCompleteLambda(DoLambda _dolamda, CompleteNotifyDoLambda completelambda,
                             const char* callerfunction, const char* callerfilename,
                             const size_t& callerfileline)
{
	_isCheckedCompleteLambda = true;

	_lambda = _dolamda;
	_completeLambda = completelambda;
	_callerFunction = callerfunction;
	_callerFile = callerfilename;
	_callerFileLine = callerfileline;
}

bool Task::IsEmptyCompleteLambda() const
{
	return _isCheckedCompleteLambda;
}

void Task::Do() const
{
	_lambda();
}

void Task::CompleteDo() const
{
	_completeLambda();
}
