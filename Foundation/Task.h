#pragma once

/*
	Task�� �۾��� ����������,
	������ �Ӽ��� �� �����ʴ´�.

	Task�� TaskAgent�� ���� ���������
	TaskAgent�� TaskManager�� ���� ���������.

	Task�� ���ٸ� ��´�.

	������ �߿��� ��ɸ� ������ �ִ� ���ۿ� ���� Task Ŭ����

*/

namespace Util
{
	using DoLambda = std::function<void()>;
	using CompleteNotifyDoLambda = std::function<void()>;

	class Task final
	{
	private:
		DoLambda _lambda;
		CompleteNotifyDoLambda _completeLambda;

		bool _isCheckedCompleteLambda{};

		std::string _callerFunction;
		std::string _callerFile;
		size_t _callerFileLine{};
	public:
		Task(DoLambda doLambda, const char* callerFunction,
			const char* callerFilename, const size_t& callerFileLine);

		Task(DoLambda doLambda, CompleteNotifyDoLambda completeLambda,
			const char* callerFunction, const char* callerFilename,
			const size_t& callerFileLine);

		Task() = default;
		~Task() = default;

		void SetLambda(DoLambda dolamda, const char* callerfunction,
			const char* callerfilename, const size_t& callerfileline);
		void SetCompleteLambda(DoLambda _dolamda, CompleteNotifyDoLambda completelambda,
			const char* callerfunction, const char* callerfilename,
			const size_t& callerfileline);
		bool IsEmptyCompleteLambda() const;

		void Do() const;
		void CompleteDo() const;
	};

}