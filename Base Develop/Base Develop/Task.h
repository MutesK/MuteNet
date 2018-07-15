#pragma once

/*
	Task�� �۾��� ����������,
	������ �Ӽ��� �� �����ʴ´�.

	Task�� TaskAgent�� ���� ���������
	TaskAgent�� TaskManager�� ���� ���������.

	Task�� ���ٸ� ��´�.

	������ �߿��� ��ɸ� ������ �ִ� ���ۿ� ���� Task Ŭ����

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

