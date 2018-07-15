#pragma once

/*
	Task는 작업의 단위이지만,
	스레드 속성이 들어가 있지않는다.

	Task는 TaskAgent에 의해 만들어지고
	TaskAgent는 TaskManager에 의해 만들어진다.

	Task는 람다를 담는다.

	지금은 중요한 기능만 가지고 있는 뼈밖에 없는 Task 클래스

*/

using dolamda = std::function<void()>;
using completeNotifyDolamda = std::function<void()>;

class Task
{
public:
	Task(dolamda _dolamda, const char* callerfunction,
		const char* callerfilename, const size_t& callerfileline);

	Task(dolamda _dolamda, completeNotifyDolamda completelambda,
		const char* callerfunction, const char* callerfilename,
		const size_t& callerfileline);

	Task();
	~Task();

	void SetLambda(dolamda _dolamda, const char* callerfunction,
		const char* callerfilename, const size_t& callerfileline);
	void SetCompleteLambda(dolamda _dolamda, completeNotifyDolamda completelambda,
		const char* callerfunction, const char* callerfilename,
		const size_t& callerfileline);
	bool IsEmptyCompeteLambda();

	void Do();
	void CompleteDo();
private:
	dolamda  _lambda;
	completeNotifyDolamda  _completelambda;

	bool		_isCheckedCompleteLambda;

	std::string _callerfunction;
	std::string _callerfile;
	size_t _callerfileline;
};

